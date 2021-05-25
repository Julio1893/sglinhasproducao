#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STR_SIZE				256
#define LISTA_MAX				500
#define FICHEIRO_BASEDEDADOS	"basededados.bin"

typedef struct LinhaProducao_str{
	char nomeLinha[STR_SIZE];
	char descricao[STR_SIZE];
}LinhaDeProducao;

typedef struct Maquina_str{
	char codigo[STR_SIZE];
	char nome[STR_SIZE];
	int ano_compra;
	char estado[STR_SIZE];
	LinhaDeProducao * linha;
	char codigo_maquina_substituta[STR_SIZE];
	char nomeLinha[STR_SIZE];
}Maquina;

typedef struct BaseDeDados_str{
	Maquina maquinas[LISTA_MAX];
	int n_maquinas;
	int n_linhas;
	LinhaDeProducao linha[LISTA_MAX];
}BaseDeDados;


/**
 * @brief Método que permite abrir o ficheiro binário e carregar a estrutura para a memoria
 *
 * @param nomeF Nome do ficheiro da base de dados
 * @param bd Estrutura da base de dados
 * @return void
 */
 
void basededados_ler(char * nomeF,BaseDeDados * bd){
	int i,j;
	FILE *fp = fopen(nomeF,"r");
	
	if(fp == NULL){
		printf("Alerta: Ficheiro nao existe!\n");
		return;
	}
	
	fread(bd,sizeof(BaseDeDados),1,fp);
	
	fclose(fp);
	
	for(i=0;i<bd->n_maquinas;i++){
		for(j=0;j<bd->n_linhas;j++){
			if(strcmp(bd->maquinas[i].nomeLinha,bd->linha[j].nomeLinha) == 0){
				bd->maquinas[i].linha = &(bd->linha[j]);
				
			}
		}
	}
	
}


/**
 * @brief Método que permite Armazenar a estrutura da base de dados em um ficheiro binario.
 *
 * @param nomeF Nome do ficheiro da base de dados
 * @param bd Estrutura da base de dados
 * @return void
 */
 
void basededados_gravar(char * nomeF, BaseDeDados * db){
	FILE *fp = fopen(nomeF,"w");
	
	if(fp == NULL){
		printf("Erro: Gravar Ficheiro!\n");
		return;
	}
	
	fwrite(db,sizeof(BaseDeDados),1,fp);
	
	fclose(fp);
}


/**
 * @brief Método que permite traduzir uma estrutura do tipo LinhaDeProducao e escrever na consola de output
 *
 * @param LinhaDeProducao Estrutura que presenta a linha de producao
 * @return void
 */
 
void linhaDeProducao_imprimir(LinhaDeProducao * linha){
	
	printf("Linha de Producao: ");
	if(linha != NULL){
		printf("%s\n",linha->nomeLinha);
	}else{
		printf("None\n");
	}
	
}

/**
 * @brief Método que permite traduzir uma estrutura do tipo maquina e escrever na consola de output
 *
 * @param Maquina Estrutura que presenta a Maquina
 * @return void
 */
 
void maquina_imprimir(Maquina * maquina){
	printf("Codigo: %s\n\tNome: %s\n\tAno Compra: %d\n\tEstado: %s\n\t",
		maquina->codigo,
		maquina->nome,
		maquina->ano_compra,
		maquina->estado
	);
	if(strcmp(maquina->estado,"Substituido") == 0){
		printf("\tMaquina: %s\n\t",maquina->codigo_maquina_substituta);
	}
	
	linhaDeProducao_imprimir(maquina->linha);
	
}

/**
 * @brief Método que permite imprimir todas as maquinas armazenadas
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void maquinas_imprimir(BaseDeDados * db){
	int i = 0;
	
	for(i=0;i<db->n_maquinas;i++){
		if(strcmp(db->maquinas[i].estado,"Removido") != 0){
			maquina_imprimir(&(db->maquinas[i]));
		}
	}
	
}

/**
 * @brief Método que permite Adicionar uma maquina
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void maquinas_adicionar(BaseDeDados * db){
	printf("Codigo: ");
	scanf("%s",db->maquinas[db->n_maquinas].codigo);
	
	printf("Nome: ");
	scanf("%s",db->maquinas[db->n_maquinas].nome);
	
	printf("Ano de Compra: ");
	scanf("%d",&(db->maquinas[db->n_maquinas].ano_compra));
	
	sprintf(db->maquinas[db->n_maquinas].estado,"Funcional");
	db->maquinas[db->n_maquinas].linha = NULL;
	
	sprintf(db->maquinas[db->n_maquinas].codigo_maquina_substituta,"");
	sprintf(db->maquinas[db->n_maquinas].nomeLinha,"");
	
	db->n_maquinas++;
}

/**
 * @brief Método que permite remover uma maquina
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void maquinas_remover(BaseDeDados * db){
	char cod[STR_SIZE];
	int i;
	
	printf("Codigo: ");
	scanf("%s",cod);
	
	for(i=0;i<db->n_maquinas;i++){
		if(strcmp(cod,db->maquinas[i].codigo) == 0){
			sprintf(db->maquinas[i].estado,"Removido");
			printf("Removido com sucesso!\n");
			return;
		}
	}
	
	printf("Codigo nao encontrado\n");
	return;
	
}

/**
 * @brief Método que permite proceder a substituicao de uma maquina
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void maquinas_substituir(BaseDeDados * db){
	char cod_orig[STR_SIZE];
	char cod_dest[STR_SIZE];
	Maquina * origem;
	Maquina * destino;
	int i;
	
	printf("Codigo Maquina Origem: ");
	scanf("%s",cod_orig);
	
	printf("Codigo Maquina destino: ");
	scanf("%s",cod_dest);
	
	for(i=0;i<db->n_maquinas;i++){
		if(strcmp(cod_orig,db->maquinas[i].codigo) == 0){
			origem = &(db->maquinas[i]);
		}
		if(strcmp(cod_dest,db->maquinas[i].codigo) == 0){
			destino = &(db->maquinas[i]);
		}
	}
	
	if(
		destino == NULL || origem == NULL || origem == destino || 
		strcmp(origem->estado,"Removido") == 0 ||
		strcmp(destino->estado,"Removido") == 0 ||
		strcmp(origem->estado,"Substituido") == 0 ||
		strcmp(destino->estado,"Substituido") == 0
		){
		printf("Correspondencia invalida\n");
		return;
	}
	
	sprintf(origem->estado,"Substituido");
	sprintf(origem->codigo_maquina_substituta,destino->codigo);
	
	printf("Substituicao sucesso\n");
}

/**
 * @brief Método que permite Imprimir uma linha de producao
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @param LinhaDeproducao Estrutura da linha de producao a ser utilizada
 * @return void
 */
 
void linha_producao_imprimir(LinhaDeProducao * linha, BaseDeDados * db){
	int i;
	int j = 1;
	printf("Nome: %s\n\tDescricao: %s\n",linha->nomeLinha,linha->descricao);
	for(i=0;i<db->n_maquinas;i++){
		if(linha == db->maquinas[i].linha){
			printf("\tCodigo %da Maquina: %s\n",j++,db->maquinas[i].codigo);
		}	
	}
	
}

/**
 * @brief Método que permite Imprimir todas as linhas de producao armazenadas
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void linhas_producao_imprimir(BaseDeDados * db){
	int i;
	for(i=0;i<db->n_linhas;i++){
		linha_producao_imprimir(&db->linha[i],db);
	}
}

/**
 * @brief Método que permite adicionar uma nova linha de producao a base de dados
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void linha_producao_adicionar(BaseDeDados * db){
	printf("Nome: ");
	scanf("%s",db->linha[db->n_linhas].nomeLinha);
	printf("Descricao: ");
	scanf("%s",db->linha[db->n_linhas].descricao);
	
	db->n_linhas++;
	
}

/**
 * @brief Método que permite imprimir todas as linhas de producao armazenadas
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void linha_producao_listar_maquinas(BaseDeDados * db){
	
	int i,j;
	
	for(i=0;i<db->n_linhas;i++){
		printf("%d: %s\n",i,db->linha[i].nomeLinha);
	}
	
	printf("ID Linha: ");
	scanf("%d",&j);
	
	if(j > db->n_linhas || j < 0){
		printf("ID Linha Invalido!\n");
		return;
	}
	
	for(i=0;i<db->n_maquinas;i++){
		if(db->maquinas[i].linha == &(db->linha[j]))
			maquina_imprimir(&(db->maquinas[i]));
	}
}

/**
 * @brief Método que permite associar uma maquina a uma linha de producao
 *
 * @param BaseDedados Estrutura que contem toda a base de dados
 * @return void
 */
 
void linha_producao_associar_maquina(BaseDeDados * db){
	int i,j,k;
	
	for(i=0;i<db->n_linhas;i++){
		printf("%d: %s\n",i,db->linha[i].nomeLinha);
	}
	
	printf("ID Linha: ");
	scanf("%d",&j);
	
	if(j > db->n_linhas || j < 0){
		printf("ID Linha Invalido!\n");
		return;
	}
	
	for(i=0;i<db->n_maquinas;i++){
		printf("%d: ",i);
		maquina_imprimir(&(db->maquinas[i]));
	}
	
	printf("ID Maquina: ");
	scanf("%d",&k);
	
	if(k > db->n_maquinas || k < 0){
		printf("ID Maquina Invalido!\n");
		return;
	}
	
	strcpy(db->maquinas[k].nomeLinha,db->linha[j].nomeLinha);
	db->maquinas[k].linha = &db->linha[j];
	
	printf("Associacao feita com sucesso!\n");
}

/**
 * @brief Funcao Main onde maioritarimente faz o controlo do Menu
 *
 * @param argc Nao utilizado
 * @param argv Nao utilizado
 * @return int
 */
 
int main(int argc, char *argv[]) {
	BaseDeDados db;
	int sair = 0;
	int opcao;
	
	
	basededados_ler(FICHEIRO_BASEDEDADOS,&db);
	
	do{
		printf("Menu:\n\t1. Maquinas\n\t\t11. Adicionar Maquina\n\t\t12. Remover Maquina\n\t\t13. Substituir Maquina\n\t2. Linha de Producao\n\t\t21. Criar Linha\n\t\t22. Ver Maquinas de linha\n\t\t23. Associar Maquina\n\t3. Sair\n>");	
		scanf("%d",&opcao);
		
		switch(opcao){
			case 1:
				maquinas_imprimir(&db);
				break;
			case 11:
				maquinas_adicionar(&db);
				break;
			case 12:
				maquinas_remover(&db);
				break;
			case 13:
				maquinas_substituir(&db);
				break;
			case 2:
				linhas_producao_imprimir(&db);
				break;
				
			case 21:
				linha_producao_adicionar(&db);
				break;
				
			case 22:
				linha_producao_listar_maquinas(&db);
				break;
				
			case 23:
				linha_producao_associar_maquina(&db);
				break;
			case 3:
				sair = 1;
				break;
				
			default:
				printf("Opcao errada\n");
				break;
		}
		
		fflush(stdin);
		basededados_gravar(FICHEIRO_BASEDEDADOS,&db);
		
	}while(sair == 0);
	
	
	return 0;
}
