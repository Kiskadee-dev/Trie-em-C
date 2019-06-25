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
    if(profundidade < strlen(str)-1){
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
    if(profundidade < strlen(str)){
        if(verbose){
            if(profundidade == 0){
                printf("PALAVRA: [%s]\n", str);
            }
            printf("Inserir [%c], profundidade=[%d]\n", str[profundidade], profundidade);
            
        }
        Node* atual = alvo; //Alvo é o node escolhido, se inicia na raiz e desce até o fim.
        if(atual->nodes_internos[str[profundidade]] != NULL){ //A letra existe, não é necessário inserir nada, desce para o próximo nível
            if(profundidade < strlen(str)-1){
                if(verbose){
                    printf("Existe [%c]\n", str[profundidade]);
                }
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

            insere_string(str, profundidade+1, atual->nodes_internos[str[profundidade]], dados); //O node atual existe, insere o próximo dentro dele
            return 0;
        }else{ //A letra não existe, é necessário inserir
            
            //Encontra um node vago dentro do node *Usando a letra como índice por performance
            if(verbose)
                printf("Inserindo [%c]\n", str[profundidade]);
            Node *node = calloc(1, sizeof(Node));
            atual->nodes_internos[str[profundidade]] = node;
            atual = node;

            if(profundidade < strlen(str)-1){ //Não é uma folha, portanto não é uma palavra
                atual->caractere = str[profundidade];
                atual->termina=0;
                atual->profundidade=profundidade;

                insere_string(str, profundidade+1, atual, dados); //Desce mais um nível
            }else{ //se é uma folha então forma uma palavra
                atual->caractere = str[profundidade];
                atual->termina=1;
                atual->dados=dados;
                atual->profundidade = profundidade;
                if(verbose)
                    printf("Nova palavra! => %lf\n", dados);
            }
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    if(argc > 1){
        if(strcmp(argv[1], "--v") == 0){
            verbose=1;
        }
    }

    //Cria o node inicial e aloca seus nodes internos
    Node root;
    for(int i = 0; i < tamanho_nodes_internos; i++){
        root.nodes_internos[i] = malloc(sizeof(Node));
    }
   
    int N, M;
    scanf("%d %d", &N, &M);
    char cargo[2000];
    double pontos;
    for(int i = 0; i < N; i++){
    	scanf(" %s%lf", cargo, &pontos);
    	insere_string(cargo, 0, &root, pontos);
    }
    char palavra[2000];
    for(int i = 0; i < M; i++){
	pontos = 0;
	while(1){
		scanf(" %s", palavra);
		if(strcmp(palavra, ".") != 0){
			//printf("%s\n", palavra);
			pontos += checa_string(palavra, 0, &root);
		}else{
			break;
		}
	}
	printf("%.0lf\n", pontos);
    }
    return (EXIT_SUCCESS);
}
