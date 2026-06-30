#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVParser.h"

#define READ_BUF_SIZE 8192

typedef struct {
    int primeiraLinha;
    double casos;
    double mortes;
} Totais;

void processaLinha(char **cols, int ncols, void *userData) {
    Totais *t = (Totais*) userData;

    if (t->primeiraLinha) {
        t->primeiraLinha = 0;
        return;
    }

    if (ncols > 8 && strcmp(cols[1], "South America") == 0) {
        if (cols[5][0] != '\0')
            t->casos += atof(cols[5]);

        if (cols[8][0] != '\0')
            t->mortes += atof(cols[8]);
    }
}

int main() {
    FILE *arq;
    char *buffer;
    int lidos;
    CSVParser parser;
    Totais totais;

    arq = fopen("owid-covid-data.csv", "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    buffer = (char*) malloc(READ_BUF_SIZE);
    if (buffer == NULL) {
        fclose(arq);
        return 1;
    }

    CSVParser_init(&parser);

    totais.primeiraLinha = 1;
    totais.casos = 0;
    totais.mortes = 0;

    lidos = fread(buffer, 1, READ_BUF_SIZE, arq);
    while (lidos > 0) {
        CSVParser_processLines(&parser, buffer, lidos, processaLinha, &totais);
        lidos = fread(buffer, 1, READ_BUF_SIZE, arq);
    }

    CSVParser_processLines(&parser, "\n", 1, processaLinha, &totais);

    printf("Total de casos na America do Sul: %.0lf\n", totais.casos);
    printf("Total de mortes na America do Sul: %.0lf\n", totais.mortes);

    free(buffer);
    fclose(arq);
    return 0;
}