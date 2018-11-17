#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct command{
	char comando[60];
	char nome_tabela[60];
	char campos[60];
}COMMAND;

int alinhamento_coluna(char** vtr,char* anterior, int col, int lin,int x);
void separa_comando(char *comand, COMMAND *aux);
void criar_tabela(char *nome, char* campo);
int type_camp(char *campo);
int check_type(char *tipo);
int campos_check(char *campos, FILE *tab, FILE *setup);
int check_command(char *command);
int check_pk(char *campos);
void inserir_linha(char *tabela, char *valores);
int check_valor(char *valor, int i, char **tipos);
int check_int(char *valor);
int check_float(char *str);
int check_char(char *str);
char** carrega_tabela(char *nome_tab);
int checkpk_valor(char *tab, char *val, int col, int lin);
void mostrar_tabela(char *nome);
void sintax();
void listar_tabelas (char *nome);
int apagar_tabela (char *str_1, char *str_2);
int apagar_linha (char* str_name, char* str_pk);
int alfa(char* nome_tabela, char *nome_campo);
int check_tabela_existe(char* nome);
void mostrar_campos_tipos(char* nome);

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
				if(alfa(aux.nome_tabela, aux.campos)){
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
//=======================Funcao sintax==============================================================
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
//========================================CRIAR TABELA==============================================
//Faz a checagem se a chave primaria foi inserida no comando criar_tabela.
//retorna 1 caso o campo seja valido para ser chave primaria.
//retorna 0 caso o campo não seja valido.
int check_pk(char *campos){
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
int check_command(char *command){
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
//====================================cria a tebela====================================================
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
	if(check_command(campo) && check_pk(campo)){

		if(campos_check(campo, tabela, setup1)){
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

	if(check_pk(campo) == 0){
		printf("Erro: Chave primaria!\n");
	}

//Fechamento das streams.
	fclose(tabela);
	fclose(setup1);
	fclose(lista);
}
//recebe uma string e retorna a posicao do primeiro espaco na string.
//auxilia a funcao check_type
int type_camp(char *campo){
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
int check_type(char *tipo){
	char tipo_checa[16];
	int espaco = 0, aux = 0;
	espaco = type_camp(tipo);
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
int campos_check(char *campos, FILE *tab,FILE *setup){
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
		if(check_type(aux_string)){
			espaco = type_camp(aux_string)+1;
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
			espaco = type_camp(aux_string);
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
//==================================INSIRIR DADOS NA TABELA==============================================
//insere uma nova linha na tabela indica.
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
		
		if(checkpk_valor(tabela, valores, colunas,linhas)){
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
				
				if(check_valor(valor, i-1, tipos)){
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
//checa se a chave primaria e unica.
//retorna 1 caso a chave primaria seja unica, 0 caso nao.
int checkpk_valor(char *tab, char *val, int col, int lin){
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
int check_valor(char *valor, int i, char **tipos){

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
//==========================Funcoes================================================================
//exibe a tabela no terminal para o usuario.
//recebe o nome da tabela que vai ser exibida.
//nao tem retorno
void mostrar_tabela(char *nome){
	FILE *setup_tab;
	char **tabela, arquivo_setup[60];
	int colunas = 0, linhas = 0, quebra_linha = 0, alinhamento = 0;
	tabela = carrega_tabela(nome);

	strcpy(arquivo_setup, nome);
	strcat(arquivo_setup, ".setup");
	setup_tab = fopen(arquivo_setup, "r");
	if(setup_tab == NULL){
		printf("Erro na abertura do arquivo!\n");
	}else{
		fscanf(setup_tab, "%d %d\n", &colunas, &linhas);
	}
	
	for(int i = 0;i < colunas*linhas;i++){
		
		if(quebra_linha != 0 ){
			alinhamento = alinhamento_coluna(tabela,tabela[i-1],colunas,linhas,quebra_linha-1);
			for(int i = 0;i < alinhamento;i++){
				printf(" ");
			}
			printf("%s",tabela[i]);
		}else{
			printf("%s",tabela[i]);
		}
		
		quebra_linha++;
		if(quebra_linha == colunas){
			quebra_linha = 0;
			printf("\n");
		}
		
	}
//libera a memoria da alocacao dinamica e fecha a stream
	for(int i = 0;i < colunas*linhas;i++){
		free(tabela[i]);
	}
	free(tabela);
	fclose(setup_tab);
}
//calcula a quantidade de espacos necessario para alinhar as colunas da tabela.
//retorna a quantidade de espacos.
int alinhamento_coluna(char** vtr,char* anterior, int col, int lin,int x){
	int maior_tamanho = 0, aux = 0,jump = 0;

	jump = x;
	for(int i = 0;i < lin;i++){
		if(strlen(vtr[jump]) > maior_tamanho){
			maior_tamanho = strlen(vtr[jump]);
		}
		jump += col;
	}

	aux = maior_tamanho - strlen(anterior);
	aux = abs(aux);
	

	return aux+1;

}
//======================Funcao check tipo==========================================================
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
//=========================================listar tabela,apagar linha e apagar tabela==============================
void listar_tabelas (char *nome){
	char vtr[60];
	int aux;
	FILE *arquivo;
	arquivo = fopen(nome, "r");
	if (arquivo == NULL){
		printf("erro na abertura\n");
	}else{
		do{
			aux = fscanf(arquivo, "%s", vtr);
			if (aux != (-1)){
				printf("%s\n", vtr);	
			}
		}while(aux != (-1));
	}
	fclose(arquivo);
}

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
//=========================================extras====================================================
//funcao alfa: ordena alfabeticamente a tabela
//recebe como parametro um ponteiro de char que aponte para o nome da tabela
//retorna 1 se ordenor alfabeticamente
//retorna 0 se o campo escolhido nao e valido
int alfa(char* nome_tabela, char *nome_campo){
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