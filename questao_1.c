#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco 
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main(int argc, char**argv){

    FILE *f;
    int qtd;
    long pos;
    Endereco e;

    if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

    char *proc = argv[1];

	f = fopen("cep_ordenado.dat","rb");
    fseek(f, 0, SEEK_END);
    pos = ftell(f);
    qtd = pos/sizeof(Endereco);

    int inicio = 0;
    int fim = qtd-1;

    while(inicio <= fim){
        int meio = (inicio+fim)/2;

        fseek(f,meio*sizeof(Endereco), SEEK_SET);
        fread(&e,sizeof(Endereco),1,f);

        if(strncmp(proc, e.cep, 8) == 0){
            printf("CEP encontrado. Informacoes: %.72s, %.72s, %.72s, %.72s", e.logradouro, e.bairro, e.cidade, e.uf);
            return 0;
        }
        else if(strncmp(proc, e.cep, 8) < 0){
            fim = meio - 1;
        }else{
            inicio = meio +1;
        }
    }

    printf("Esse CEP nao foi encontrado :(");
    return 0;
}