#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criar_tab.h"
#include "exibicao.h"
#include "funcoes_extras.h"

typedef struct command{
	char comando[60];
	char nome_tabela[60];
	char campos[60];
}COMMAND;

void separa_comando(char *comand, COMMAND *aux);
int check_tabela_existe(char* nome);
void sintax();
int apagar_tabela (char *str_1, char *str_2);
int apagar_linha (char* str_name, char* str_pk);

int main(){
	char comando[60], pk[60];
	COMMAND aux;
	system("clear");
	printf("            SGBD\n=============================\n");
	printf(" 1 - criar_tabela\n");
	printf(" 2 - listar_tabelas\n");
	printf(" 3 - inserir_linha\n");
	printf(" 4 - mostrar_tabela\n");
	printf(" 6 - apagar_linha\n");
	printf(" 7 - apagar_tabela\n");
	printf(" 8 - sintax\n");
	printf(" 9 - Extras\n    1 - ordenar_alfa\n");
	printf("10 - limpar_tela\n");
	printf("11 - sair(encerra o programa)\n");
	printf("=============================\n");

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
			sintax();
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
		}else if(strcmp(aux.comando, "ordenar_alfa") == 0){
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
			printf("            SGBD\n==============================\n");
			printf(" 1 - criar_tabela\n");
			printf(" 2 - listar_tabelas\n");
			printf(" 3 - inserir_linha\n");
			printf(" 4 - mostrar_tabela\n");
			printf(" 6 - apagar_linha\n");
			printf(" 7 - apagar_tabela\n");
			printf(" 8 - sintax\n");
			printf(" 9 - Extras\n    1 - ordenar_alfa\n");
			printf("10 - limpar_tela\n");
			printf("11 - sair(encerra o programa)\n");
			printf("==============================\n");
		}else{
			printf("Erro: comando invalido!\n");
		}

	}while(strcmp(aux.comando, "sair") != 0);

	return 0;
}
//==============================================Funcao sintax==============================================================
//atualizar o comando sintax
void sintax(){
	printf("\nComando criar_tabela:\n");
	printf("Sintax: criar_tabela nome_tabela tipo campo1,tipo campo2*,...,tipo campoN,\n");
	printf("OBS: Os campos devem ser separados por virgula(deve ter um virgula no final)\nObrigatoriamente um dos campos deve ser chave primaria para isso sinaliza com '*' como ultimo caracte do nome do campo(o campo deve ser do tipo int)\n");
	printf("\nComando inserir_linha:\n");
	printf("Sintax: inserir_linha nome_tabela valor1,valor2,...,valorN,\nOBS: os valores devem ser digitados na mesma ordem que os campos da tabela e devem ser do mesmo tipo\n");
	printf("\nComando mostrar_tabela:\nSintax: mostrar_tabela nome_tabela\nVai exibir todos os dados da nome_tabela\n");
	printf("\n");
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

	tabelas = fopen("lista_tabelas", "r");

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
//====================================================================================================
//funcoes acima devem ficar no arquivo main.
/*checkpoint 7
retornos:
0 - execucao ok
1 - tabela nao existe
-1 - erro na abertura
str_1 eh o nome da lista de tabelas str_2 eh o nome da tabela*/
int apagar_tabela (char *str_1, char *str_2){
	FILE *file_1, *file_2;
	char tester[60], backup[60], string_aux[60];
	int aux;
	strcpy(backup, str_1);
	//abre canais
	file_1 = fopen(str_1, "r");
	file_2 = fopen(strcat(backup,".backup"), "w");
	//checa se arquivo existe
	if (file_1 == NULL){
		//comunica e fecha canais, remove backup
		printf("erro na abertura\n");
		fclose(file_1);
		fclose(file_2);
		remove(backup);
		return -1;
	}else{
		//se o arquvo existir, checa a tabela existe
		//checa o nome da tabela
		do{
			aux = fscanf(file_1, "%s", tester);
			if(strcmp(tester, str_2) == 0){
				aux = -1;
			} 
		}while(aux != (-1));
		//se a tabela existir copia para o backup as tabelas
		//exceto a excluida
		if (strcmp(tester, str_2) == 0){
			//retorna para o inicio
			fseek(file_1, 0, SEEK_SET);
			//comecando a copiar ate que chegue no fim do arquivo
			do{
				aux = fscanf(file_1, "%s", tester);
				if (strcmp(tester, str_2) != 0 && aux != EOF){
					fprintf(file_2, "%s\n", tester);
				}
			}while(aux != (-1));
			//fecha canais
			fclose(file_1);
			fclose(file_2);
			//remove a tabela e a lista
			remove(str_1);
			remove(str_2);
			//remove a .setup
			strcpy(string_aux, str_2);
			strcat(string_aux, ".setup");
			remove(string_aux);
			//renomeia o backup como o nova lista de tabelas
			rename(backup, str_1);
			return 0;
		}else{
			//se a tabela nao existir apaga backup e fecha os canais
			printf("tabela nao existe\n");
			fclose(file_1);
			fclose(file_2);
			remove(backup);
			return 1;
		}
	}
}

//fazer funcao pra checar se a pk inserida existe
//essa funcao entra com a pk na forma de string (str_pk),
//e o nome da tabela (str_name)
/*
retornos padrao 
	-1 > setup da tabela nao existe
	 1 > tabela nao existe
	 0 > RUNOK
	 2 > impossivel criar backup
*/
int apagar_linha (char* str_name, char* str_pk){
	FILE *table, *setup, *backup;
	char** lines_data;
	char aux_string[60];
	strcpy(aux_string, str_name);
	int pk_col, n_col, n_lin, aux_int;
	//abrindo o canal setup e table
	setup = fopen(strcat(aux_string, ".setup"), "r+");
	table = fopen(str_name, "r");
	//checa abertura da file
	if ((setup == NULL) || (table == NULL)){
		printf("erro na execucao\n");
		if (setup == NULL){
			printf("setup nao existe\n");
			fclose(setup);
			fclose(table);
			return 1;
		}
		printf("tabela nao existe\n");
		fclose(setup);
		fclose(table);
		return -1;
	}else{
		//recebendo o numero de colunas
		fscanf(setup, "%d %d", &n_col, &n_lin);
		//printf("%d %d\n", n_col, n_lin); >> debug
		//alocando memoria para receber os dados
		lines_data = (char**)calloc(n_col, sizeof(char*));
		for(int i = 0; i < n_col; ++i){
			*(lines_data+i) = (char*)calloc(60, sizeof(char));
		}
		//recebendo os nomes para determinar a posicao da pk
		for(int i = 0; i < n_col; ++i){
			aux_int = fscanf(table, "%s", *(lines_data+i));
			for(int a = 0; a < strlen(*(lines_data+i)); ++a){
				if(*(*(lines_data+i)+a) == '*'){
					pk_col = i;
				}
			}
		}
		//abrindo file backup pra transferir os dados
		backup = fopen("backup", "w");
		//caso erro, fechamento da execucao, libera memoria e fecha canais
		if (backup == NULL){
			printf("erro inesperado na execucao\n");
			for(int i = 0; i < n_col; ++i){
				free(*(lines_data+i));
			}
			free(lines_data);
			fclose(table);
			fclose(backup);
			fclose(setup);
			remove("backup");
			return 2;
		}else{
			//imprime a tabela no backup ate o EOF
			do{
				//checa se a string eh diferente da pk e de EOF, se sim imprime no backup
				if(strcmp(*(lines_data+pk_col), str_pk) != 0 && aux_int != (-1)){
					for(int i = 0; i < n_col; ++i){
						fprintf(backup, "%s ", *(lines_data+i));
					}
					for(int i = 0; i < n_col; ++i){
						aux_int = fscanf(table, "%s", *(lines_data+i));
					}
					//se for diferente de EOF pula linha
					if(aux_int != (-1)){
						fprintf(backup, "\n");
					}
				}else{
					if(strcmp(*(lines_data+pk_col), str_pk) == 0){
						//se chegar a esse ponto entao a pk existe e 
						//vai ser eliminada da tabela, logo
						//pode diminuir a as linhas do setup
						fseek(setup, 0, SEEK_SET);
						fprintf(setup, "%d %d\n", n_col, (n_lin-1));
					}
					//se chegar aqui eh pq uma das condicoes nao foi satisfeita, provavelmente
					//a pk eh igual a inserida, caso contrario, EOF, o programa teria encerrado
					for(int i = 0; i < n_col; ++i){
						aux_int = fscanf(table, "%s", *(lines_data+i));
					}
				}
			}while(aux_int != (-1));
		}
		//liberando memoria e fechando canal
		for(int i = 0; i < n_col; ++i){
			free(*(lines_data+i));
		}
		free(lines_data);
		fclose(table);
		fclose(setup);
		fclose(backup);
		//tornando a backup a nova tabela
		remove(str_name);
		rename("backup", str_name);
		return 0;
	}
}