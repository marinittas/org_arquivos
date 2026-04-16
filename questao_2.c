#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

typedef struct {
    char cep[8];
    long pos;
} Indice;

int compara(const void *e1, const void *e2){
    return strncmp(((Indice*)e1)->cep, ((Indice*)e2)->cep, 8);
}

int main(int argc, char *argv[]) {

    if(argc != 2){
        fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
        return 1;
    }

    char *proc = argv[1];

    FILE *f = fopen("cep_ordenado.dat", "rb");
    if(!f){
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    fseek(f,0,SEEK_END);
    long total = ftell(f)/sizeof(Endereco);
    rewind(f);

    Indice *vet = malloc(total * sizeof(Indice));

    for(long i = 0; i <total; i++){
        Endereco e;
        fread(&e, sizeof(Endereco), 1, f);

        strncpy(vet[i].cep, e.cep, 8);
        vet[i].pos = i;
    }

    qsort(vet,total,sizeof(Indice),compara);

    FILE *g = fopen("indice.dat", "wb");
    fwrite(vet, sizeof(Indice), total, g);
    fclose(g);

    long inicio = 0;
    long fim = total -1;

    while(inicio <= fim){
        long meio = (inicio + fim) / 2;

        if(strncmp(proc, vet[meio].cep, 8) == 0){

            long pos = vet[meio].pos;

            fseek(f, pos*sizeof(Endereco), SEEK_SET);

            Endereco e;
            fread(&e,sizeof(Endereco),1,f);

            printf("CEP encontrado. Info: %.72s, %.72s, %.72s, %.72s, %.2s,%.8s ",e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);

            free(vet);
            fclose(f);
            return 0;
        }
        else if(strncmp(proc,vet[meio].cep, 8) < 0){
            fim = meio - 1;
        }
        else{
            inicio = meio + 1;
        }
    }

    printf("Esse CEP não foi encontrado :(\n");

    free(vet);
    fclose(f);
    return 0;
}