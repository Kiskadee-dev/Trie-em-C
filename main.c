/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
   char caractere;
   int tamanho_nodes_internos;
   struct node* nodes_internos[256];
   int dados;
   int termina;
   int profundidade;
} Node;

int tamanho_root = 1;

int insere_string(char *str, int profundidade, Node* alvo){
    if(profundidade < strlen(str)){
        printf("palavra = [%s] Tentando inserir [%c], profundidade=[%d]\n", str, str[profundidade], profundidade);
        Node* atual = alvo; //Alvo é o node escolhido, se inicia no root e depois desce até o fim.
        int novo = 1;
            for(int i = 0; i < alvo->tamanho_nodes_internos; i++){ //Por hora, o root tem apenas 1 espaço
                if(atual->nodes_internos[i]->caractere == str[profundidade]){
                    printf("Existe [%c]\n", str[profundidade]);
                    insere_string(str, profundidade+1, atual->nodes_internos[i]);
                    novo = 0;
                    break;
                }
            }

        if(novo){
            int cheio = 1;
            //Escolhe um node vago dentro do node
            printf("%d nodes internos\n", alvo->tamanho_nodes_internos);
            printf("tentando inserir dentro de [%c]\n", alvo->caractere);
            for(int i = 0; i < alvo->tamanho_nodes_internos; i++){
                if(atual->nodes_internos[i]->caractere == '\0'){
                    printf("Lugar vago, inserindo [%c]\n", str[profundidade]);
                    atual = atual->nodes_internos[i];
                    cheio = 0;
                    break;
                }else{
                    printf("[%c] diz conter: %c\n", atual->caractere, atual->nodes_internos[i]->caractere);
                }
            }
            if(cheio){
                printf("Cheio.\n"); //Não existem nodes vagos
            }else if(profundidade < strlen(str)-1){ //Não é uma folha
                atual->caractere = str[profundidade];
                atual->termina=0;
                atual->profundidade=profundidade;
                atual->tamanho_nodes_internos=256;
                for(int i = 0; i < atual->tamanho_nodes_internos; i++){
                    Node* n = malloc(sizeof(Node));
                    n->caractere = '\0';
                    atual->nodes_internos[i] = n;
                }
                insere_string(str, profundidade+1, atual);
            }else{ //se é uma folha então forma uma palavra
                atual->caractere = str[profundidade];
                atual->termina=1;
                atual->profundidade = profundidade;
                atual->tamanho_nodes_internos=256;
                for(int i = 0; i < atual->tamanho_nodes_internos; i++){
                    Node* n = malloc(sizeof(Node));
                    n->caractere = '\0';
                    atual->nodes_internos[i] = n;
                }
            }
        }else{

        }
    }
    return 1;
}

int main(int argc, char** argv) {
    char input[2000] = "ana";
    Node root;
    root.tamanho_nodes_internos = 1;
    *root.nodes_internos = calloc(1, sizeof(Node));
    insere_string(input, 0, &root);
    printf("\n");
    strcpy(input, "anajulia");
    insere_string(input, 0, &root);
    return (EXIT_SUCCESS);
}

