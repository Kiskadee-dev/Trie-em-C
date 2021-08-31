/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int tamanho_nodes_internos = 256;

typedef struct node{
   char caractere;
   struct node* nodes_internos[256];
   char dados[300];
   int termina;
   int profundidade;
} Node;

int verbose=0;
char BUFFER[2000];

void limpa_arvore(Node* node, int profundidade){ //Desaloca os nodes da memória, incluindo o raiz. depth first
    for(int i = 0; i < tamanho_nodes_internos; i++){
        if(node->nodes_internos[i] != NULL){
            limpa_arvore(node->nodes_internos[i], profundidade+1);
        }
    }
    if(verbose){
        printf("Limpando: [%c]\n", node->caractere);
    }
    free(node);
}

void mostra_tudo(Node* node, int profundidade, char *buffer){
    for(int i = 0; i < tamanho_nodes_internos; i++){
        if(node->nodes_internos[i] != NULL){ //Se tem um caractere
            buffer[profundidade] = node->nodes_internos[i]->caractere; //adiciona ele ao buffer
            if(node->nodes_internos[i]->termina){ //Faz o print da palavra e retorna
                buffer[profundidade+1] = '\0';
                printf("%s %s\n", buffer, node->nodes_internos[i]->dados);
            }
            mostra_tudo(node->nodes_internos[i], profundidade+1, buffer);
        }
    }
    //Se remove do buffer
    buffer[profundidade] = '\0';
}

int checa_string(char *str, int profundidade, Node* node){
    if(str[profundidade+1] != '\0'){
        if(node->nodes_internos[str[profundidade]] != NULL){
            node = node->nodes_internos[str[profundidade]];
            return checa_string(str, profundidade+1, node);
        }else{
            return 0;
        }
    }else if(node->nodes_internos[str[profundidade]] != NULL){
        if(node->nodes_internos[str[profundidade]]->termina){
            printf("%s\n", node->nodes_internos[str[profundidade]]->dados);
            return (EXIT_SUCCESS); //Encontrou a palavra
        }
        else{
            return (EXIT_FAILURE); //Não forma uma palavra
        }
    }else{
        return (EXIT_FAILURE); //Fim da string a verificar
    }
}

int insere_string(char *str, int profundidade, Node* alvo, char* dados){
    if(str[profundidade] != '\0'){
        if(verbose){
            if(profundidade == 0){
                printf("PALAVRA: [%s]\n", str);
            }
            printf("Inserir [%c], profundidade=[%d]\n", str[profundidade], profundidade);
            
        }
        Node* atual = alvo; //Alvo é o node escolhido, se inicia na raiz e desce até o fim.
        if(atual->nodes_internos[str[profundidade]] != NULL){ //A letra existe, não é necessário inserir nada, desce para o próximo nível
            if(str[profundidade+1] != '\0'){
                if(verbose){
                    printf("Existe [%c]\n", str[profundidade]);
                }
                insere_string(str, profundidade+1, atual->nodes_internos[str[profundidade]], dados); //O node atual existe, insere o próximo dentro dele
                return (EXIT_SUCCESS);
            }else{
               if(!atual->nodes_internos[str[profundidade]]->termina){ //É uma letra existente mas é uma folha, portanto uma palavra nova
                   atual->nodes_internos[str[profundidade]]->termina = 1;
                   strcpy(atual->nodes_internos[str[profundidade]]->dados, dados);

                    if(verbose){
                       printf("Existe [%c], mas eh uma palavra nova\n", str[profundidade]);
                    }
                }else{
                    if(verbose){
                       printf("[%c] faz parte da palavra\n", str[profundidade]);
                    }
                    return (EXIT_SUCCESS);
                }
            }
        }else{ //A letra não existe, é necessário inserir
            
            //Encontra um node vago dentro do node *Usando a letra como índice por performance
            if(verbose)
                printf("Inserindo [%c]\n", str[profundidade]);
            Node *node = calloc(1, sizeof(Node));
            atual->nodes_internos[str[profundidade]] = node;
            atual = node;

            atual->caractere = str[profundidade];
            if(str[profundidade+1] != '\0'){ //Não é uma folha, portanto não é uma palavra
                atual->profundidade=profundidade;
                insere_string(str, profundidade+1, atual, dados); //Desce mais um nível
            }else{ //se é uma folha então forma uma palavra
                atual->termina = 1;
                strcpy(atual->dados, dados);
                atual->profundidade = profundidade;
                if(verbose)
                    printf("Nova palavra! => %s\n", dados);
            }
        }
    }
    return (EXIT_FAILURE);
}

int main(int argc, char** argv) {
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(strstr(argv[i], "--v")){
                verbose = 1;
                break;
            }
        }
    }
    //Cria o node inicial e aloca seus nodes internos
    Node* root = calloc(1, sizeof(Node));
    
   char linguagem[2000];
   char traducao[2000];
   int N, M;
   scanf("%d", &N);
   for(int i = 0; i < N; i++){
   	scanf(" %s", linguagem);
   	scanf(" %[^\n]s", traducao);
   	insere_string(linguagem, 0, root, traducao);
   }
   
   char nome[300];
   scanf("%d", &M);
   for(int i = 0; i < M; i++){
   	scanf(" %[^\n]s", nome);
   	scanf("%s", linguagem);
   	printf("%s\n", nome);
   	checa_string(linguagem, 0, root);
   	printf("\n");
   }
    return (EXIT_SUCCESS);
}
