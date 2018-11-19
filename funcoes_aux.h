#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//se as funcoes desse arquivo ja tiverem sido escritas no aquivos main os include guards vao previnir que elas sejam escritas novamente.
#ifndef FUNCOES_AUX
#define FUNCOES_AUX
char** carrega_tabela(char *nome_tab);
int check_int(char *valor);
int check_float(char *str);
int check_char(char *str);

//carrega a tabela na memoria
//retorna um ponteiro de ponteiro de char(retorna um pointeiro para um vetor de string).
//retorna o vetor que contem todos os dados da tebela.
char** carrega_tabela(char *nome_tab){
	FILE *tabela, *setup_tab;
	char setup[60], **matriz;
	int colunas = 0, linhas = 0;

	strcpy(setup, nome_tab);
	strcat(setup, ".setup");
	tabela = fopen(nome_tab, "r");
	if(tabela == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
	setup_tab = fopen(setup, "r");
	if(setup_tab == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
//lendo do arquivo tabela.setup  a qtd. de colunas e linhas.
	fseek(setup_tab, 0, SEEK_SET);
	fscanf(setup_tab, "%d %d\n",&colunas, &linhas);
//alocando a memoria para a matriz
	matriz = (char**) malloc((colunas*linhas)*sizeof(char*));
	if(matriz == NULL){
		printf("Erro na memoria!\n");
	}else{
		for(int i = 0;i < colunas*linhas;i++){
			matriz[i] = (char*) malloc(1*sizeof(setup));
		}
	}
//copiando os dados da tabela para a matriz
	for(int i = 0;i < colunas*linhas;i++){
		fscanf(tabela, "%s ", matriz[i]);
	}
//fechando as streams
	fclose(tabela);
	fclose(setup_tab);

	return matriz;
}
//=========================================Funcao check tipo==========================================
//checa se a string eh composta por inteiro;
// -1 > erro, 0 > eh int, 1 > nao eh int
int check_int(char *str){
	int aux = -1;
	for (int i = 0; i < strlen(str); ++i){
		if (*(str+i) >= 48 && *(str+i) <= 57){
			aux = 0;
		}
		else{
			return 1;
		}
	}
	return aux;
}
//checa se a string representa um valor float;
// -1 > erro, 0 > eh int, 1 > nao eh int
int check_float(char *str){
	int dotcont = 0, aux = -1;
	for (int i = 0; i < strlen(str); ++i){
		if (*(str+i) >= 48 && *(str+i) <= 57){
			aux = 0;
		}else{
			if (*(str+i) == 46){
				dotcont++;
			}
			else{
				return 1;
			}
		}
		if (dotcont > 1){
			return 1;
		}
	}
	return aux;
}
/*checa se a string tem um unico elemento, caso sim retorna 0.
caso contrario retorna 0. caso erro retorna -1*/
int check_char(char *str){
	if (strlen(str) != 1){
		return 1;
	}else{
		return 0;
	}
	return -1;
}
#endif //FUNCOES_AUX