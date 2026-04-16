#include <stdio.h>
#include <stdlib.h>
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

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

void intercala(char *nomeA, char *nomeB, char *arqSaida)
{
    FILE *a, *b, *saida;
    Endereco ea, eb;

	a = fopen(nomeA,"rb");
	b = fopen(nomeB,"rb");
	saida = fopen(arqSaida,"wb");

	fread(&ea,sizeof(Endereco),1,a);
	fread(&eb,sizeof(Endereco),1,b);

	while(!feof(a) && !feof(b))
	{
		if(strncmp(ea.cep, eb.cep, 8) < 0)
		{
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);
		}
		else
		{
			fwrite(&eb,sizeof(Endereco),1,saida);
			fread(&eb,sizeof(Endereco),1,b);
		}
	}

	while(!feof(a))
	{
		fwrite(&ea,sizeof(Endereco),1,saida);
		fread(&ea,sizeof(Endereco),1,a);
	}

	while(!feof(b))
	{
		fwrite(&eb,sizeof(Endereco),1,saida);
		fread(&eb,sizeof(Endereco),1,b);
	}

	fclose(a);
	fclose(b);
	fclose(saida);
}

int main()
{
	FILE *f;
	Endereco *e;

	int k = 4; //pd mudar isso
	long pos, qtd, tamanhoBloco;

	f = fopen("cep.dat","rb"); //nao ordenado. ainda usa rb?
    if(f == NULL){
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

	fseek(f,0,SEEK_END);
	pos = ftell(f);
	qtd = pos/sizeof(Endereco);
	rewind(f);

	tamanhoBloco = qtd/k;

	for(int i = 0; i<k; i++)
	{
		long tamAtual;

		if(i == k-1)
			tamAtual = qtd - (i *tamanhoBloco);
		else
			tamAtual = tamanhoBloco;

		e = (Endereco*) malloc(tamAtual *sizeof(Endereco));
		fread(e, sizeof(Endereco), tamAtual, f);

		qsort(e,tamAtual,sizeof(Endereco),compara);

		char nome[20];
		sprintf(nome, "bloco%d.dat", i); //preenche o nome q é o buffer com o nome do arquivo e o numero dele do lado

		FILE *g = fopen(nome, "wb");
		fwrite(e,sizeof(Endereco),tamAtual,g);
		fclose(g);

		free(e);
	}

	fclose(f);

	int numBlocos = k;
	int cont = 0;

	while(numBlocos > 1)
	{
		int novoNum = 0;
		for(int i = 0; i < numBlocos; i += 2){
			char nomeA[20], nomeB[20], nomeSaida[20];

			sprintf(nomeA, "bloco%d.dat", i);
			sprintf(nomeB, "bloco%d.dat", i+1);
			sprintf(nomeSaida, "bloco_temp%d.dat", novoNum);

			intercala(nomeA, nomeB, nomeSaida);
			novoNum++;
		}

		//renomear arquivos temp
		for(int i = 0; i < novoNum; i++){
			char antigo[20], novo[20];

			sprintf(antigo, "bloco_temp%d.dat", i);
			sprintf(novo, "bloco%d.dat", i);

			remove(novo);
			rename(antigo, novo);
		}

		numBlocos = novoNum;
		cont++;
	}

	printf("arquivo final ordenado: bloco0.dat");

	return 0;
}