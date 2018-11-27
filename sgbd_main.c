#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_aux.h"
#include "criar_inserir.h"
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
void limpa_comando(COMMAND *aux);
void exibir_menu();

int main(){
	char comando[60], extra[60];
	COMMAND aux;
	system("cls");
	exibir_menu();
	do{
		printf("Digite o comando >>> ");
		fscanf(stdin, "%[^\n]",comando);
		setbuf(stdin, NULL);
		separa_comando(comando, &aux);

		if(aux.nome_tabela[0] != '\0' && strcmp(aux.nome_tabela, "NULL") != 0){
			if(strcmp(aux.comando, "criar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					criar_tabela(aux.nome_tabela, aux.campos);
					limpa_comando(&aux);
				}else{
					printf("Tabela %s ja existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}
			}else if(strcmp(aux.comando, "inserir_linha") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(aux.campos[0] == '\0'){
						mostrar_campos_tipos(aux.nome_tabela);
						printf("Insira os valores:\n");
						fscanf(stdin, "%[^\n]", aux.campos);
						setbuf(stdin, NULL);
						inserir_linha(aux.nome_tabela, aux.campos);
						limpa_comando(&aux);
					}else{
						inserir_linha(aux.nome_tabela, aux.campos);
						limpa_comando(&aux);
					}
				}
			}else if(strcmp(aux.comando, "mostrar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					mostrar_tabela(aux.nome_tabela);
					limpa_comando(&aux);
				}
			}else if(strcmp(aux.comando, "apagar_linha") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(apagar_linha (aux.nome_tabela, aux.campos) == 0){
						limpa_comando(&aux);
						printf("Linha apagada com sucesso!\n");
					}
				}
			}else if(strcmp(aux.comando, "apagar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(apagar_tabela("lista_tabelas", aux.nome_tabela) == 0){
						limpa_comando(&aux);
						printf("Tabela apagada com sucesso!\n");
					}
				}
			}else if(strcmp(aux.comando, "ordenar_cres") == 0 || strcmp(aux.comando, "ordenar_des") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(strcmp(aux.comando, "ordenar_cres") == 0){
						if(ordenar(aux.nome_tabela, aux.campos, 0)){
							printf("Tabela ordena com sucesso!\n");
							limpa_comando(&aux);
						}else{
							printf("O campo escolhido nao e valido!\n");
							limpa_comando(&aux);
						}
					}else{
						if(ordenar(aux.nome_tabela, aux.campos, 1)){
							limpa_comando(&aux);
						}else{
							printf("O campo escolhido nao e valido!\n");
							limpa_comando(&aux);
						}
					}
				}
			}else if(strcmp(aux.comando, "inserir_coluna") == 0){
				if(inserir_coluna(aux.nome_tabela, aux.campos)){
					printf("Coluna criada com sucesso!\n");
					limpa_comando(&aux);
				}else{
					printf("Erro de sintaxe!\n");
					mostrar_sintaxe_correta(aux.comando);
					limpa_comando(&aux);
				}
			}else{
				printf("Erro: comando invalido!\n");
			}
		}else{
			if(strcmp(aux.comando, "sintax") == 0){
				if(aux.nome_tabela[0] == '\0'){
					printf("Sintaxe: sintax comando\n");
					limpa_comando(&aux);
				}else{
					sintax(aux.nome_tabela);
					limpa_comando(&aux);
				}
			}else if(strcmp(aux.comando, "listar_tabelas") == 0){
				listar_tabelas("lista_tabelas");
				limpa_comando(&aux);
			}else if(strcmp(aux.comando, "sair") == 0){
				printf("programa encerrado\n");
			}else if(strcmp(aux.comando, "limpar_tela") == 0){
				system("cls");
				limpa_comando(&aux);
				exibir_menu();
			}else{
				printf("Erro: comando invalido!\n");
				mostrar_sintaxe_correta(aux.comando);
			}
		}

	}while(strcmp(aux.comando, "sair") != 0);

	return 0;
}
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
	printf(" 8 - Extras\n    1 - ordenar_cres\n    2 - ordenar_des\n    3 - sintax\n    4 - inserir_coluna\n");
	printf(" 9 - limpar_tela\n");
	printf("10 - sair\n");
	printf("=============================\n");
}
//atribui a string "NULL" para todos os campos da struct do tipo COMMAND
//recebe uma struct do tipo COMMAND
void limpa_comando(COMMAND *aux){
	strcpy(aux->comando, "NULL");
	strcpy(aux->nome_tabela, "NULL");
	strcpy(aux->campos, "NULL");
}
