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
    FILE *f;
    ArvoreB *indice;
    Endereco reg;
    char chave[TAM_CHAVE + 1];
    long pos = 0;

    indice = ArvoreB_Abre("arvore.dat");
    f = fopen("a2.dat", "rb");

    if (f == NULL) {
        printf("Erro ao abrir a2.dat\n");
        return 1;
    }

    while (fread(&reg, sizeof(Endereco), 1, f) == 1) {
        sprintf(chave, "%.8s", reg.cep);
        ArvoreB_Insere(indice, chave, pos);
        pos++;
    }

    fclose(f);
    ArvoreB_Fecha(indice);

    return 0;
}