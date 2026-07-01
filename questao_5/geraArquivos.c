#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    FILE *origem;
    FILE *dest1;
    FILE *dest2;
    Endereco reg;
    double x;

    origem = fopen("cep.dat", "rb");
    dest1 = fopen("a1.dat", "wb");
    dest2 = fopen("a2.dat", "wb");

    if (origem == NULL || dest1 == NULL || dest2 == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    srand(time(NULL));

    while (fread(&reg, sizeof(Endereco), 1, origem) == 1) {
        x = (double) rand() / RAND_MAX;
        if (x < 0.8)
            fwrite(&reg, sizeof(Endereco), 1, dest1);

        x = (double) rand() / RAND_MAX;
        if (x < 0.8)
            fwrite(&reg, sizeof(Endereco), 1, dest2);
    }

    fclose(origem);
    fclose(dest1);
    fclose(dest2);

    return 0;
}