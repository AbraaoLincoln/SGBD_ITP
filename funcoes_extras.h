#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_aux.h"

int alfabeticamente(char* nome_tabela, char *nome_campo);

//=========================================extras====================================================
//funcao alfa: ordena alfabeticamente a tabela
//recebe como parametro um ponteiro de char que aponte para o nome da tabela
//retorna 1 se ordenor alfabeticamente
//retorna 0 se o campo escolhido nao e valido
int alfabeticamente(char* nome_tabela, char *nome_campo){
	FILE *tabela, *tabela_setup;
	char **tab, **tab_copia, **pk,*aux_ptr, tab_setup[60], aux_string[16];
	int colunas = 0, linhas = 0;
	int pk_campo = 0,campo = 0,jump = 0, jump2 = 0, aux = 0;
	int check_campo = -1;

	strcpy(tab_setup, nome_tabela);
	strcat(tab_setup, ".setup");
//abertura dos arquivos tabela e tabela.setup
	tabela = fopen(nome_tabela, "r+");
	if(tabela == NULL){
		printf("Erro na abertura do arquivo!\n");
	} 
	tabela_setup = fopen(tab_setup, "r");
	if(tabela_setup == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
//carregando os dados dos arquivos para as variaveis
	tab = carrega_tabela(nome_tabela);
	fscanf(tabela_setup, "%d %d", &colunas, &linhas);
	//verifica se o campo e valido para ser ordenado alfabeticamente
	for(int i = 0;i < colunas;i++){
		if(strcmp(tab[i], nome_campo) == 0){
			check_campo = i;
			break;
		}
	}
	if(check_campo == -1){
		return 0;
		fclose(tabela);
		fclose(tabela_setup);
	}
	for(int i = 0; i <= check_campo;i++){
		fscanf(tabela_setup, "%s ", aux_string);
	}
	if(strcmp(aux_string, "string") != 0){
		if(strcmp(aux_string, "char") != 0){
			return 0;
			fclose(tabela);
			fclose(tabela_setup);
		}
	}
//criando um vetor so com as chaves primarias
	//encontrando o campo da cheve primaria
	for(int i = 0;i < colunas;i++){
		for(int j = 0;j < strlen(tab[i]);j++){
			if(tab[i][j] == '*'){
				pk_campo = i;
				break;
			}
		}
		if(pk_campo != 0){
			break;
		}
	}
	//copiando as chaves primarias para o vetor pk
	pk = (char**) malloc((linhas-1)*sizeof(char*));
	if(pk ==  NULL){
		printf("Erro na memoria!\n");
	}else{
		for(int i = 0;i < linhas-1;i++){
			pk[i] = (char*) malloc(1*sizeof(tab_setup));
		}
		jump = pk_campo + colunas;
		for(int i = 0;i < linhas-1;i++){
			pk[i] = tab[jump];
			jump += colunas;
		}
	}
	//fazendo uma copia do vetor tab para o tab_copia
	tab_copia = (char**) malloc((colunas*linhas)*sizeof(char*));
	if(tab_copia == NULL){
		printf("Erro na memoria!\n");
	}else{
		for(int i = 0;i < colunas*linhas;i++){
			tab_copia[i] = (char*) malloc(1*sizeof(tab_setup));
		}
		for(int i = 0;i < colunas*linhas;i++){
			strcpy(tab_copia[i], tab[i]);
		}
	}
	//transformando todas as strings em lower case
	//encontra a posicao do campo no vetor
	for(int i = 0;i < colunas;i++){
		if(strcmp(tab_copia[i], nome_campo) == 0){
			campo = i;
			break;
		}
	}

	jump = campo + colunas;
	for(int i = 0;i < linhas-1;i++){
		for(int j = 0;j < strlen(tab_copia[jump]);j++){
			if(tab_copia[jump][j] > 40 && tab_copia[jump][j] < 91){
				tab_copia[jump][j] = tab_copia[jump][j] + 32;
			}
		}
		jump += colunas;
	}

	//comparando as strings
	jump = campo + colunas;
	jump2 = campo + colunas;
	for(int i = 0;i < linhas-1;i++){
		for(int j = 0;j < linhas-1;j++){
			if(strcmp(tab_copia[jump],tab_copia[jump2]) < 0){
				aux_ptr = tab_copia[jump];
				tab_copia[jump] = tab_copia[jump2];
				tab_copia[jump2] = aux_ptr;
				aux_ptr = pk[i];
				pk[i] = pk[j];
				pk[j] = aux_ptr;
			}
			jump2 += colunas;
		}
		jump += colunas;
		jump2 = campo + colunas;
	}

	//salvando no arquivo ordenado alfabeticamente
	fseek(tabela, 0, SEEK_SET);
	for(int i = 0;i < colunas;i++){
		fprintf(tabela, "%s ",tab[i]);
	}
	fprintf(tabela, "\n");
	jump = pk_campo + colunas;
	for(int i = 0;i < linhas-1;i++){
		for(int j = 0;j < linhas-1;j++){
			if(strcmp(pk[i], tab_copia[jump]) == 0){
				aux = (jump - colunas*(j+1));
				aux = jump - aux;
				for(int k = 0;k < colunas;k++){
					fprintf(tabela, "%s ",tab[aux]);
					aux++;
				}
				break;
			}
			jump += colunas;
		}
		jump = pk_campo + colunas;
		fprintf(tabela, "\n");
	}
//liberando a memoria e fechando os canais de comunicacao
	//memoria
	for(int i = 0;i < linhas-1;i++){
		free(pk[i]);
	}
	free(pk);
	for(int i = 0;i < linhas-1;i++){
		free(tab_copia[i]);
	}
	free(tab_copia);
	//canais de comunicacao
	fclose(tabela);
	fclose(tabela_setup);
	return 1;
}