#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes_aux.h"

void criar_tabela(char *nome, char* campo);
int primeiro_espaco(char *campo);
int tipo_campo(char *tipo);
int checa_campos(char *campos, FILE *tab, FILE *setup);
int checa_sintaxe_comando(char *command);
int checa_pk(char *campos);
void inserir_linha(char *tabela, char *valores);
int checa_pk_valor(char *tab, char *val, int col, int lin);
int checa_valor(char *valor, int i, char **tipos);
void mostrar_campos_tipos(char* nome);


//====================================Criar tabela===================================================
//cria a tabela e os arquivos nome_tabela, tabela.setup e lista_tabelas
//nao tem retorno.
void criar_tabela(char *nome, char* campo){
	FILE *tabela, *setup1, *lista;
	char setup[60];
//Criacao dos arquivos tabela(nome) e setup.
	strcpy(setup, nome);
	tabela = fopen(nome, "w");
	if(tabela == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
	setup1 = fopen(strcat(setup, ".setup"), "w");
	if(setup1 == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
	lista = fopen("lista_tabelas", "a+");
	if(lista == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
//Salvando nos arquivos os campos e seus tipos.
//checa se os campos sao validos, se sim criar a tabela, caso contrario nao.
	if(checa_sintaxe_comando(campo) && checa_pk(campo)){

		if(checa_campos(campo, tabela, setup1)){
			printf("Tabela criada com sucesso!\n");
			fprintf(lista, "%s\n", nome);
		}else{
			remove(nome);
			remove(setup);
		}
	}else{
		remove(nome);
		remove(setup);
		printf("Erro de Sintax!\n");
	}

	if(checa_pk(campo) == 0){
		printf("Erro: Chave primaria!\n");
	}

//Fechamento das streams.
	fclose(tabela);
	fclose(setup1);
	fclose(lista);
}
//recebe uma string e retorna a posicao do primeiro espaco na string.
//auxilia a funcao tipo_campo
int primeiro_espaco(char *campo){
	int i = 0;
	
	for( i = 0;i < strlen(campo);i++){
		if(campo[i] == ' '){
			break;
		}
	}
	
	return i;
}
//Checa se o tipo do campo que o usuario esta tentando inserir e valido.
//retorna 1 caso o campo seja valido, 0 caso nao.
int tipo_campo(char *tipo){
	char tipo_checa[16];
	int espaco = 0, aux = 0;
	espaco = primeiro_espaco(tipo);
	for(int i = 0;i < espaco;i++){
		tipo_checa[i] = tipo[i];
		aux++;
	}
	tipo_checa[aux] = '\0';
	
	if(strcmp(tipo_checa, "char") == 0){
		return 1;
	}else if(strcmp(tipo_checa, "int") == 0){
		return 1;
	}else if(strcmp(tipo_checa, "float") == 0){
		return 1;
	}else if(strcmp(tipo_checa, "string") == 0){
		return 1;
	}
	return 0;
}
//checa se os campos que o usuario esta tentando adicionar sao todos validos.
//se os campos forem validos a tabela e criada(tabela.setup tambem).
//retorna 1 caso todos os campos sejam validos, 0 caso nao. 
int checa_campos(char *campos, FILE *tab,FILE *setup){
	int y = 0, conta = 0, virgula = -1, vir_commando = 0, vir_ultima = 0, espaco = 0, aux = 0;
	char aux_string[60] = {0}, aux_string1[60] = {0}, tipo[16];

	for(int i = 0;i < strlen(campos);i++){
		if(campos[i] == ','){
			conta++;
		}
	}
	fprintf(setup,"%d %d\n",conta, 1);
	for(int i = 1; i <= conta;i++){

		for(int j = 0;j < strlen(campos);j++){
			if(campos[j] == ','){
				vir_commando++;
				if(vir_commando == i){
					vir_ultima = virgula;
					virgula = j;
					vir_commando = 0;
					break;
				}
			}

		}

		for(int z = vir_ultima+1;z < virgula;z++){
			aux_string[y] = campos[z];
			y++;
		}
		aux_string[y] = '\0';
		y = 0;
		if(tipo_campo(aux_string)){
			espaco = primeiro_espaco(aux_string)+1;
			for(int x = espaco;x < strlen(aux_string);x++){
				aux_string1[y] = aux_string[x];
				y++;
			}
			aux_string1[y] = '\0';
			fprintf(tab, "%s ",aux_string1);
			y = 0;
			if(aux_string1[y] == '\0'){
				printf("Erro de sintax!\n");
				return 0;
			}
//salvando dados no arquivo tabela.setup 
			espaco = primeiro_espaco(aux_string);
			for(int i = 0;i < espaco;i++){
				tipo[i] = aux_string[i];
				aux++;
			}
			tipo[aux] = '\0';
			aux = 0;
			fprintf(setup, "%s ",tipo);
		}else{
			printf("Erro: tipo invalido!\n");
			return 0;
		}
	}
	fprintf(tab, "\n");
	return 1;
}
//Faz a checagem se a chave primaria foi inserida no comando criar_tabela.
//retorna 1 caso o campo seja valido para ser chave primaria.
//retorna 0 caso o campo não seja valido.
int checa_pk(char *campos){
	int asterisco = 0,aux = 0;

	for(int i = 0;i < strlen(campos);i++){
		if(campos[i] == '*'){
			asterisco++;
			aux = i;
		}
	}
	
	if(asterisco == 1){
		while(campos[aux] != ','){
			aux--;
			if(aux == 0){break;}
		}
		
		if(aux == 0){
			if(campos[aux] == 'i' && campos[aux+1] == 'n' && campos[aux+2] == 't'){
				return 1;
			}else{
				return 0;
			}
		}else{
			aux++;
			if(campos[aux] == 'i' && campos[aux+1] == 'n' && campos[aux+2] == 't'){
				return 1;
			}else{
				return 0;
			}
		}
	}else{
		return 0;
	}
}
//checa se a sintax do comando para inserir os campos esta correto.
//retorna 1 se a sintax estive correta, 0 caso nao. 
int checa_sintaxe_comando(char *command){
	int conta_virgula = 0, conta_espaco = 0;

	for(int i = 0;i < strlen(command);i++){
		if(command[i] == ','){
			conta_virgula++;
		}

		if(command[i] == ' '){
			conta_espaco++;
		}
	}

	if(conta_virgula == conta_espaco){
		return 1;
	}else{
		return 0;
	}
}
//======================================-inserir Linha-================================================
//insere uma nova linha na tabela indicada.
//nao tem retorno.
void inserir_linha(char *tabela, char *valores){
	FILE *setup, *tab;
	char arquivo_setup[60], **tipos, aux[16], **entrada_usuario;
	char valor[60];
	int colunas = 0, linhas = 0, conta_virgula = 0, conta = 0;
	int ultima_virgula = 0, atual_virgula = -1, virgula = 0, aux_indice = 0;
//lendo do arquivo tabela.setup o numero de colunas e linhas.
	strcpy(arquivo_setup, tabela);
	strcat(arquivo_setup, ".setup");
	setup = fopen(arquivo_setup, "r+");
	if(setup == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
	fscanf(setup, "%d %d\n", &colunas, &linhas);
	tab = fopen(tabela, "a+");
	if(tab == NULL){
		printf("Erro na abertura do arquivo!\n");
	}
//armazenando na memoria os tipo de cada coluna da tabela.
	tipos = (char**) malloc(colunas*sizeof(char*));
	if(tipos == NULL){
		printf("Erro na memoria!\n");
	}else{
		for(int i = 0;i < colunas;i++){
			tipos[i] = malloc(1*sizeof(aux));
		}
		for(int i = 0;i < colunas;i++){
			fscanf(setup, "%s", tipos[i]);
		}
	}
	
	entrada_usuario = (char**) malloc(colunas*sizeof(char*));
	if(entrada_usuario == NULL){
		printf("Erro na memoria!\n");
	}else{
		for(int i = 0;i < colunas;i++){
			entrada_usuario[i] = malloc(1*sizeof(arquivo_setup));
		}
	}

//verificando se a linha e valida se sim adiciona no arquivo, caso contrario retorna erro e nao adiciona a linha
	//codigo repetido fazer uma funcao.
	for(int i = 0;i < strlen(valores);i++){
			if(valores[i] == ','){
				conta_virgula++;
			}
		}

	if(valores[strlen(valores)-1] == ',' && conta_virgula == colunas){
		
		if(checa_pk_valor(tabela, valores, colunas,linhas)){
			for(int i = 1;i <= conta_virgula;i++){

				for(int j = 0;j < strlen(valores);j++){
					if(valores[j] == ','){
						virgula++;
						if(virgula == i){
							ultima_virgula = atual_virgula;
							atual_virgula = j;
							virgula = 0;
							break;
						}
					}
				}

				for(int z = ultima_virgula+1;z < atual_virgula;z++){
					valor[aux_indice] = valores[z];
					aux_indice++;
				}
				valor[aux_indice] = '\0';
				aux_indice = 0;
				
				if(checa_valor(valor, i-1, tipos)){
					printf("Erro de sintax\n");
					printf("O Valor informado não é do tipo do campo(%s)\n", tipos[i-1]);
				}else{
					conta++;
					strcpy(entrada_usuario[i-1], valor);
				}
			
			}

			if(conta == conta_virgula){
				for(int i = 0;i < colunas;i++){
					fprintf(tab, "%s ", entrada_usuario[i]);
				}
				fprintf(tab, "\n");
				fseek(setup, 0, SEEK_SET);
				linhas++;
				fprintf(setup, "%d %d\n", colunas, linhas);
				printf("Linha inserida com sucesso!\n");
			}

		}else{
			printf("Chave primaria invalida!\n");
		}

	}else{
		printf("Erro de sintax\n");
	}

//fechando o canal de comunicacao do arquivo
	fclose(setup);
	fclose(tab);
//liberando a memoria da alocacao dinamica.
	for(int i = 0;i < colunas;i++){
		free(entrada_usuario[i]);
	}
	free(entrada_usuario);
	for(int i = 0;i < colunas;i++){
		free(tipos[i]);
	}
	free(tipos);	
}

//checa se a chave primaria e unica.
//retorna 1 caso a chave primaria seja unica, 0 caso nao.
int checa_pk_valor(char *tab, char *val, int col, int lin){
	char **valores, pk_valor[60];
	int pk_campo = 0, conta = 0, virgula = 0, z = 0,jump = 0;

	valores = carrega_tabela(tab);

	for(int i = 0;i < col;i++){
		for(int j = 0;j < strlen(valores[i]);j++){
			if(valores[i][j] == '*'){
				pk_campo = i;
				break;
			}
		}
	}

	for(int i = 0;i < strlen(val);i++){
		if(val[i] == ','){
			conta++;
		}

		if(conta == pk_campo+1){
			virgula = i-1;

			while(val[virgula] != ','){

				if(virgula != -1){
					virgula--;
				}else{
					break;
				}
			}
			virgula++;
			while(val[virgula] != ','){
				pk_valor[z] = val[virgula];
				z++;
				virgula++;
			}
			pk_valor[z] = '\0';
			break;
		}
		
	}

	jump = col + pk_campo;
	for(int i = 1;i < lin;i++){
		if(strcmp(pk_valor, valores[jump]) == 0){
			return 0;
		}
		jump += col;
	}
	
	for(int i = 0;i < col*lin;i++){
		free(valores[i]);
	}
	free(valores);
	
	return 1;
}
//checa se o valor que o usuario passou e do tipo do campo.
//retorna 0 caso o valor seja do tipo do campo, 1 caso nao.
int checa_valor(char *valor, int i, char **tipos){

	if(strcmp(tipos[i], "int") == 0){
		if(check_int(valor)){
			return 1;
		}else{
			return 0;
		}
	}else if(strcmp(tipos[i], "char") == 0){
		if(check_char(valor)){
			return 1;
		}else{
			return 0;
		}
	}else if(strcmp(tipos[i], "float") == 0){
		if(check_float(valor)){
			return 1;
		}else{
			return 0;
		}
	}else if(strcmp(tipos[i], "string") == 0){
		return 0;
	}


}
//mostra os campos e seus tipos para o usuario
//nao tem retorno.
void mostrar_campos_tipos(char* nome){
	FILE *tabela, *setup_tab;
	char tab_setup[60], campo[60], tipo[16];
	int colunas = 0, linhas = 0;
	//abertura dos aquivos
	strcpy(tab_setup, nome);
	strcat(tab_setup, ".setup");
	setup_tab = fopen(tab_setup, "r");
	if(setup_tab == NULL){
		printf("Erro na abertura do arquivo\n");
	}else{
		fscanf(setup_tab, "%d %d\n", &colunas, &linhas);
	}
	tabela = fopen(nome, "r");
	if(tabela == NULL){
		printf("Erro na abertura do arquivo\n");
	}
	//exibindo na tela os campos e seus tipos
	for(int i = 0;i < colunas;i++){
		fscanf(tabela, "%s ", campo);
		fscanf(setup_tab, "%s ", tipo);
		printf("%s(%s) ", campo, tipo);
	}
	printf("\n");
	//fechamento dos canais de comunicacao
	fclose(setup_tab);
	fclose(tabela);
}