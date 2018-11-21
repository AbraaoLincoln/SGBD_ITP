#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_aux.h"
#include "criar_tab.h"
#include "apagar.h"
#include "busca.h"
#include "exibicao.h"
#include "funcoes_extras.h"

typedef struct command{
	char comando[60];
	char nome_tabela[60];
	char campos[60];
}COMMAND;

void separa_comando(char *comand, COMMAND *aux);
int check_tabela_existe(char* nome);
//void sintax();
void exibir_menu();

int main(){
	char comando[60];
	COMMAND aux;
	system("clear");
	exibir_menu();
	do{
		printf("Digite o comando >>> ");
		fscanf(stdin, "%[^\n]",comando);
		setbuf(stdin, NULL);
		separa_comando(comando, &aux);

		if(strcmp(aux.comando, "criar_tabela") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				criar_tabela(aux.nome_tabela, aux.campos);
			}else{
				printf("Tabela %s ja existe\n", aux.nome_tabela);
			}
		}else if(strcmp(aux.comando, "inserir_linha") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				printf("Tabela %s nao existe\n", aux.nome_tabela);
			}else{
				if(aux.campos[0] == '\0'){
					mostrar_campos_tipos(aux.nome_tabela);
					printf("Insira os valores:\n");
					fscanf(stdin, "%[^\n]", aux.campos);
					setbuf(stdin, NULL);
					inserir_linha(aux.nome_tabela, aux.campos);
				}else{
					inserir_linha(aux.nome_tabela, aux.campos);
				}
			}
		}else if(strcmp(aux.comando, "mostrar_tabela") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				printf("Tabela %s nao existe\n", aux.nome_tabela);
			}else{
				mostrar_tabela(aux.nome_tabela);
			}
		}else if(strcmp(aux.comando, "apagar_linha") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				printf("Tabela %s nao existe\n", aux.nome_tabela);
			}else{
				if(apagar_linha (aux.nome_tabela, aux.campos) == 0){
					printf("Linha apagada com sucesso!\n");
				}
			}
		}else if(strcmp(aux.comando, "sintax") == 0){
			//sintax();
		}else if(strcmp(aux.comando, "listar_tabelas") == 0){
			listar_tabelas("lista_tabelas");
		}else if(strcmp(aux.comando, "apagar_tabela") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				printf("Tabela %s nao existe\n", aux.nome_tabela);
			}else{
				if(apagar_tabela("lista_tabelas", aux.nome_tabela) == 0){
					printf("Tabela apagada com sucesso!\n");
				}
			}
		}else if(strcmp(aux.comando, "sair") == 0){
			printf("programa encerrado\n");
		}else if(strcmp(aux.comando, "ordenar_afb") == 0){
			if(check_tabela_existe(aux.nome_tabela)){
				printf("Tabela %s nao existe\n", aux.nome_tabela);
			}else{
				if(alfabeticamente(aux.nome_tabela, aux.campos)){
					printf("Tabela ordena alfabeticamente com sucesso!\n");
				}else{
					printf("O campo escolhido nao e valido!\n");
				}
			}
		}else if(strcmp(aux.comando, "limpar_tela") == 0){
			system("clear");
			exibir_menu();
		}else{
			printf("Erro: comando invalido!\n");
		}

	}while(strcmp(aux.comando, "sair") != 0);

	return 0;
}
//==============================================Funcao sintax==============================================================
//atualizar o comando sintax
/*
void sintax(){
	printf("\nComando criar_tabela:\n");
	printf("Sintax: criar_tabela nome_tabela tipo campo1,tipo campo2*,...,tipo campoN,\n");
	printf("OBS: Os campos devem ser separados por virgula(deve ter um virgula no final)\nObrigatoriamente um dos campos deve ser chave primaria para isso sinaliza com '*' como ultimo caracte do nome do campo(o campo deve ser do tipo int)\n");
	printf("\nComando inserir_linha:\n");
	printf("Sintax: inserir_linha nome_tabela valor1,valor2,...,valorN,\nOBS: os valores devem ser digitados na mesma ordem que os campos da tabela e devem ser do mesmo tipo\n");
	printf("\nComando mostrar_tabela:\nSintax: mostrar_tabela nome_tabela\nVai exibir todos os dados da nome_tabela\n");
	printf("\n");
}
*/
//==============================funcoa para os comandos=============================================
//separa a string comando em comando, nome_tabela e campos
//recebe uma variavel do tipo command e uma string com os camandos
void separa_comando(char *comand, COMMAND *aux){
	int espaco1 = 0,espaco2 = 0,j = 0, conta_espaco = 0;

	for(int i = 0;i < strlen(comand);i++){
		if(comand[i] == ' '){
			espaco1 = i;
			break;
		}
	}
	
	for(int i = 0;i < strlen(comand);i++){
		if(comand[i] == ' '){
			conta_espaco++;
			if(conta_espaco == 2){
				espaco2 = i;
				break;
			}
		}
	}

	if(espaco2 == 0){
		espaco2 = strlen(comand);
	}
	
	if(espaco1 != 0){

		for(int i = 0;i < espaco1;i++){
			aux->comando[i] = comand[i];
			j++;
		}
		aux->comando[j] = '\0';
		j=0;
		for(int i = espaco1+1;i < espaco2;i++){
			aux->nome_tabela[j] = comand[i];
			j++;
		}
		aux->nome_tabela[j] = '\0';
		j=0;
		for(int i = espaco2+1;i < strlen(comand);i++){
			aux->campos[j] = comand[i];
			j++;
		}
		aux->campos[j] = '\0';
	}else{
		strcpy(aux->comando, comand);
	}
	
}
//checa se a tabela existe
//retorna 1 se a tabela não existe
//retorna 0 caso a tabela exista
int check_tabela_existe(char* nome){
	FILE *tabelas;
	char tab_nome[60];
	int aux = 0;

	tabelas = fopen("lista_tabelas", "a+");

	if(tabelas == NULL){
		printf("Erro na abertura do arquivo\n");
	}else{
		while(aux != -1){
			aux = fscanf(tabelas, "%s\n", tab_nome);
			if(strcmp(tab_nome, nome) == 0){
				return 0;
			}
		}
	}
	fclose(tabelas);
	return 1;
}

//exibe o menu
void exibir_menu (){
	printf("            SGBD\n=============================\n");
	printf(" 1 - criar_tabela\n");
	printf(" 2 - listar_tabelas\n");
	printf(" 3 - inserir_linha\n");
	printf(" 4 - mostrar_tabela\n");
	printf(" 5 - buscar_valor\n");
	printf(" 6 - apagar_linha\n");
	printf(" 7 - apagar_tabela\n");
	//printf(" 8 - sintax\n");
	printf(" 8 - Extras\n    1 - ordenar_afb\n");
	printf(" 9 - limpar_tela\n");
	printf("10 - sair\n");
	printf("=============================\n");
}
