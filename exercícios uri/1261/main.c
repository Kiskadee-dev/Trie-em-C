/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int tamanho_nodes_internos = 258;

typedef struct node{
   char caractere;
   struct node* nodes_internos[258];
   double dados;
   int termina;
   int profundidade;
} Node;

int verbose=0;
char BUFFER[2000];

void mostra_tudo(Node* node, int profundidade, char *buffer){
    for(int i = 0; i < tamanho_nodes_internos; i++){
        if(node->nodes_internos[i] != NULL){ //Se tem um caractere
            buffer[profundidade] = node->nodes_internos[i]->caractere; //adiciona ele ao buffer
            if(node->nodes_internos[i]->termina){ //Faz o print da palavra e retorna
                buffer[profundidade+1] = '\0';
                printf("%s %lf\n", buffer, node->nodes_internos[i]->dados);
            }
            mostra_tudo(node->nodes_internos[i], profundidade+1, buffer);
        }
    }
    //Se remove do buffer
    buffer[profundidade] = '\0';
}

double checa_string(char *str, int profundidade, Node* node){
    if(str[profundidade+1] != '\0'){
        if(node->nodes_internos[str[profundidade]] != NULL){
            node = node->nodes_internos[str[profundidade]];
            return checa_string(str, profundidade+1, node);
        }else{
            return 0;
        }
    }else if(node->nodes_internos[str[profundidade]] != NULL){
        if(node->nodes_internos[str[profundidade]]->termina)
            return node->nodes_internos[str[profundidade]]->dados;
        else
            return 0; //Não forma uma palavra
    }else{
        return 0; //Fim da string a verificar
    }
}

int insere_string(char *str, int profundidade, Node* alvo, double dados){
    if(str[profundidade] != '\0'){
        if(verbose){
            if(profundidade == 0){
                printf("PALAVRA: [%s]\n", str);
            }
            printf("Inserir [%c], profundidade=[%d]\n", str[profundidade], profundidade);
            
        }
        Node* atual = alvo;
	if(atual->nodes_internos[str[profundidade]] != NULL){
		if(str[profundidade+1] != '\0'){
		    if(verbose){
			printf("Existe [%c]\n", str[profundidade]);
		    }
		    insere_string(str, profundidade+1, atual->nodes_internos[str[profundidade]], dados); //O node atual existe, insere o próximo dentro dele
		    return (EXIT_SUCCESS);
	    }else{
	       if(!atual->nodes_internos[str[profundidade]]->termina){ //É uma letra existente mas é uma folha, portanto uma palavra nova
		   atual->nodes_internos[str[profundidade]]->termina = 1;
		   atual->nodes_internos[str[profundidade]]->dados = dados;

		    if(verbose){
		       printf("Existe [%c], mas eh uma palavra nova\n", str[profundidade]);
		    }
		}else{
		    if(verbose){
		       printf("[%c] faz parte da palavra\n", str[profundidade]);
		    }
		}
		
	    }
        }else{ //A letra não existe, é necessário inserir
            
            //Encontra um node vago dentro do node *Usando a letra como índice por performance
            if(verbose)
                printf("Inserindo [%c]\n", str[profundidade]);
	
	    atual->nodes_internos[str[profundidade]] = calloc(1, sizeof(Node));;
            atual = atual->nodes_internos[str[profundidade]];

	    atual->caractere = str[profundidade];

            if(str[profundidade+1] != '\0'){ //Não é uma folha, portanto não é uma palavra
                insere_string(str, profundidade+1, atual, dados); //Desce mais um nível
            }else{ //se é uma folha então forma uma palavra
                atual->termina=1;
                atual->dados=dados;
                atual->profundidade = profundidade;
                if(verbose)
                    printf("Nova palavra! => %lf\n", dados);
            }
        }
    return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}


int main(int argc, char** argv) {

    //Cria o node inicial e aloca seus nodes internos
    Node* root = calloc(1, sizeof(Node));
   
    int N, M;
    scanf("%d %d", &N, &M);
    char cargo[2000];
    double pontos;
    for(int i = 0; i < N; i++){
    	scanf(" %s%lf", cargo, &pontos);
    	insere_string(cargo, 0, root, pontos);
    }
    char palavra[2000];
    for(int i = 0; i < M; i++){
	pontos = 0;
	while(1){
		scanf(" %s", palavra);
		if(strcmp(palavra, ".") != 0){
			//printf("%s\n", palavra);
			pontos += checa_string(palavra, 0, root);
		}else{
			break;
		}
	}
	printf("%.0lf\n", pontos);
    }
    return (EXIT_SUCCESS);
}
