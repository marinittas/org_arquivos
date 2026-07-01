#include <stdio.h>
#include "ArvoreB.h"

typedef struct _Endereco Endereco;

struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int main() {
    FILE *f1;
    FILE *saida;
    ArvoreB *indice;
    Endereco reg;
    char chave[TAM_CHAVE + 1];
    long achou;

    f1 = fopen("a1.dat", "rb");
    saida = fopen("Intersecoes.dat", "wb");
    indice = ArvoreB_Abre("arvore.dat");

    if (f1 == NULL || saida == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    while (fread(&reg, sizeof(Endereco), 1, f1) == 1) {
        sprintf(chave, "%.8s", reg.cep);
        achou = ArvoreB_Busca(indice, chave);

        if (achou != -1)
            fwrite(&reg, sizeof(Endereco), 1, saida);
    }

    fclose(f1);
    fclose(saida);
    ArvoreB_Fecha(indice);

    return 0;
}