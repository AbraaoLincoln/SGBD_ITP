
void sintax(char *comando);
int alfabeticamente(char* nome_tabela, char *nome_campo);
int inserir_coluna(char *nome_tabela, char *campo);

//=========================================extras====================================================
//funcao alfabeticamente: ordena alfabeticamente a tabela
//recebe o nome da tabela e o nome do campo pelo qual a tabela vai ser ordenada
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
		fprintf(stderr, "Erro na abertura do arquivo");
	} 
	tabela_setup = fopen(tab_setup, "r");
	if(tabela_setup == NULL){
		printf("Erro na abertura do arquivo!\n");
		fprintf(stderr, "Erro na abertura do arquivo");
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
		fprintf(stderr, "Erro na memoria");
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
		fprintf(stderr, "Erro na memoria");
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
//funcao sintax
//recebe o comando
//exibi na tela a sintaxe do comaando

void sintax(char *comando){

	if(strcmp(comando, "criar_tabela") == 0){
		printf("Comando criar_tabela:\n\n");
		printf("sintaxe : criar_tabela nome_tabela tipo nome_campo1,int nome_campo2*,...,tipo nome_campoN,\n");
		printf("\nOBS: o nome da tabela nao deve conter espacos, tipo deve ser os tipos primitivos da linguagem C (char, int, float) ou string ");
		printf("os nomes dos campos não pode ter espacos e devem ser separados por virgulas(inclusive apos o ultimo campo).\n\n");
	}else if(strcmp(comando, "inserir_linha") == 0){
		printf("Comando inserir_linha:\n\n");
		printf("Sintaxe 1: inserir_linha nome_tabela valor1,valor2,valor3,...,valorN,\n");
		printf("\nOBS: os valores devem ser separados por virgulas(inclusive apos o ultimo valor) e devem estar na mesma ordem que os campos da tabela e ser do tipo do respectivos campos.\n\n");
		printf("Sintaxe 2: inserir_linha nome_tabela \n");
		printf("\nSera mostrado os campos da tabela e seus tipos, na mesma ordem em que estao na tabela\n");
		printf("inseria os valores onde se pede.\n");
		printf("\nOBS: os valores devem ser separados por virgulas(inclusive apos o ultimo valor) e devem estar na mesma ordem que os campos da tabela e ser do tipo do respectivos campos.\n\n");
	}else if(strcmp(comando, "mostrar_tabela") == 0){
		printf("Comando mostra_tabela:\n\n");
		printf("Sintaxe : mostrar_tabela nome_tabela\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos.\n\n");
	}else if(strcmp(comando, "buscar_valor") == 0){
		printf("comando ainda vai ser definido!\n\n");
	}else if(strcmp(comando, "apagar_linha") == 0){
		printf("Comando apagar_linha:\n\n");
		printf("Sintaxe: apagar_linha nome_tabela chave primaria da linha\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos e a chave primaria deve existir\n\n");
	}else if(strcmp(comando, "apagar_tabela") == 0){
		printf("Comando apagar_tabela:\n\n");
		printf("Sintaxe : apagar_tabela nome_tabela\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos.\n\n");
	}else if(strcmp(comando, "ordenar_afb") == 0){
		printf("Comando ordenar_afb:\n\n");
		printf("Sintaxe: ordenar_afb nome_tabela nome_campo\n");
		printf("\nOBS: o nome da tabela e do campo nao pode conter espacos, o campo dever dos tipo char ou string.\n");
		printf("Ordena a tabela de forma crescente\n\n");
	}else if(strcmp(comando, "inserir_coluna") == 0){
		printf("Comando inserir_coluna\n\n");
		printf("Sintaxe: inserir_coluna nome_tabela tipo nome_novoCampo\n");
		printf("\nOBS: o nome da tabela nao pode ter espaco, tipo deve ser um dos tipos primitivos da linguagem C (char, int, float) ou string, ");
		printf("o tipo e o nome do novo campo devem ser separando por espaco. O novo campo nao pode ser defenido como chave primaria(uso do caracter '*').\n\n");
	}else{
		printf("Comando inexistente\n");
	}
}
//funcao inserir_coluna
//recebe o nome da tabela e o novo campo
//retorna 1 se a coluna foi criada com sucesso, 0 em caso de erro

int inserir_coluna(char *nome_tabela, char *campo){
	FILE *tab, *tab_setup;
	char **tipos, **tabela, **entrada_usuario, tipo[16], nome_campo[60], tabela_setup[60], valores[100];
	int colunas = 0, linhas = 0, espaco = 0, aux = 0, conta_colunas = 0, conta = 0;

	if(tipo_campo(campo)){
		tabela = carrega_tabela(nome_tabela);
		//separando o tipo e o nome do campo nas strigs tipo e nome_campo
		espaco = primeiro_espaco(campo);
		for(int i = 0;i < espaco;i++){
			tipo[i] = campo[i];
			aux++;
		}
		tipo[aux] = '\0';
		aux = 0;
		espaco++;
		for(int i = espaco;i < strlen(campo);i++){
			nome_campo[aux] = campo[i];
			aux++;
		}
		nome_campo[aux] = '\0';
		//verifica se o nome do novo campo esta definido como chave primaria
		//se sim, retorna 0 e encerra a funcao
		for(int i = 0;i < strlen(nome_campo);i++){
			if(nome_campo[i] == '*'){
				return 0;
			}
		}
		//abrindo os canais de comunicacao com os arquivos tabela e tabela.setup
		strcpy(tabela_setup, nome_tabela);
		strcat(tabela_setup, ".setup");

		tab_setup = fopen(tabela_setup, "r+");
		if(tab_setup == NULL){
			fprintf(stderr, "Erro na abertura do arquivo!");
		}else{
			fscanf(tab_setup, "%d %d\n", &colunas, &linhas);
			tipos = (char**) malloc((colunas+1)*sizeof(char*));
			if(tipos == NULL){
				fprintf(stderr, "Erro na memoria");
			}else{
				for(int i = 0;i <= colunas;i++){
					tipos[i] = (char*) malloc(1*sizeof(tipo));
					if(i != colunas){
						fscanf(tab_setup, "%s ", tipos[i]);
					}
				}
			}
		}
		strcpy(tipos[colunas], tipo);
		tab = fopen(nome_tabela, "w");
		//verifica se a quantidade de valores passado e igual a de campos da tabela
		if(linhas > 1){
			//alocando a memoria para guadar os valores da nova coluna
			entrada_usuario = (char**) malloc((linhas-1)*sizeof(char*));
			if(entrada_usuario == NULL){
				fprintf(stderr, "Erro na memoria\n");
				return 0;
			}else{
				for(int i = 0;i < linhas-1;i++){
					entrada_usuario[i] = malloc(1*sizeof(tabela_setup));
				}
			}

			printf("Insira os valores:\n");
			fscanf(stdin, "%[^\n]", valores);
			setbuf(stdin, NULL);
			conta = checa_valor(valores, entrada_usuario, tipos, colunas);
			if(conta != linhas-1){
				return 0;
			}
		}
		//salvando a tabela com o novo campo
		if(linhas > 1){
			aux = 0;
			for(int i = 0;i < colunas*linhas;i++){
				if(i == colunas){
					fprintf(tab, "%s\n", nome_campo);
					conta_colunas = 1;
				}
				fprintf(tab, "%s ", tabela[i]);
				if(conta_colunas == colunas){
					fprintf(tab, "%s\n", entrada_usuario[aux]);
					conta_colunas = 0;
					aux++;
				}
				conta_colunas++;
			}
		}else{
			for(int i = 0;i < colunas;i++){
				fprintf(tab, "%s ", tabela[i]);
				if(i == colunas-1){
					fprintf(tab, "%s\n", nome_campo);
				}
			}
		}
		colunas++;
		//salvando a nova quantidade de colunas e o tipo no arquivo .setup da tabela
		fseek(tab_setup, 0, SEEK_SET);
		fprintf(tab_setup, "%d %d\n", colunas, linhas);
		for(int i = 0;i < colunas;i++){
			fprintf(tab_setup, "%s ", tipos[i]);
		}
		//fechando os canais de comunicacao e liberando a memoria
			//memoria
			for(int i = 0;i < colunas;i++){
				free(tipos[i]);
			}
			free(tipos);
			for(int i = 0;i < (colunas-1)*linhas;i++){
				free(tabela[i]);
			}
			free(tabela);		
			if(linhas > 1){
				for(int i = 0;i < linhas-1;i++){
					free(entrada_usuario[i]);
				}
				free(entrada_usuario);
			}			
			//streams
			fclose(tab_setup);
			fclose(tab);
	}else{
		return 0;
	}
	return 1;
}
