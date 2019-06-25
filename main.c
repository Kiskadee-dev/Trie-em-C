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
   int dados;
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
                printf("%s %d\n", buffer, node->nodes_internos[i]->dados);
            }
            mostra_tudo(node->nodes_internos[i], profundidade+1, buffer);
        }
    }
    //Se remove do buffer
    buffer[profundidade] = '\0';
}

int checa_string(char *str, int profundidade, Node* node){
    if(profundidade < strlen(str)-1){
        if(node->nodes_internos[str[profundidade]] != NULL){
            node = node->nodes_internos[str[profundidade]];
            return checa_string(str, profundidade+1, node);
        }else{
            return 0;
        }
    }else if(node->nodes_internos[str[profundidade]] != NULL){
        if(node->nodes_internos[str[profundidade]]->termina)
            return 1;
        else
            return 0; //Não forma uma palavra
    }else{
        return 0; //Fim da string a verificar
    }
}

int insere_string(char *str, int profundidade, Node* alvo, int dados){
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
                    printf("Nova palavra! => %d\n", dados);
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
    Node* root = calloc(1, sizeof(Node));
    
    char input[1000] = "Ana";
    int score;
    while(scanf("%s", input) != EOF){
        scanf("%d", &score);
        insere_string(input, 0, root, strlen(input));
        if(verbose)
            printf("\n");
    }
    char str[] = "A";
    while (scanf("%s", str) != EOF){
        printf("se %s existe: [%d]\n", str, checa_string(str, 0, root));
    }
    printf("---------------------\n");
    mostra_tudo(root, 0, BUFFER);
    return (EXIT_SUCCESS);
}