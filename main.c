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
    printf("palavra = [%s] Tentando inserir [%c], profundidade=[%d]\n", str, str[profundidade], profundidade);
    Node* atual = alvo;
    int novo = 1;
    for(int i = 0; i < alvo->tamanho_nodes_internos; i++){
        if(atual->nodes_internos[i]->caractere == str[profundidade]){
            printf("existe %c\n", str[profundidade]);
            insere_string(str, profundidade+1, atual->nodes_internos[i]);
            novo = 0;
            break;
        }
    }
    if(novo){
        int cheio = 1;
        for(int i = 0; i < alvo->tamanho_nodes_internos; i++){
            if(atual->nodes_internos[i]->caractere == '\0'){
                atual = atual->nodes_internos[i];
                cheio = 0;
                break;
            }
        }
        if(cheio){
            printf("Cheio.\n");
        }else if(profundidade < strlen(str)-1){
            atual->caractere = str[profundidade];
            atual->termina=0;
            atual->profundidade=profundidade;
            atual->tamanho_nodes_internos=256;
            struct node n;
            n.caractere = '\0';
            for(int i = 0; i < atual->tamanho_nodes_internos; i++){
                atual->nodes_internos[i] = &n;
            }
            insere_string(str, profundidade+1, atual);
        }else{
            atual->caractere = str[profundidade];
            atual->termina=1;
            atual->profundidade = profundidade;
            atual->tamanho_nodes_internos=256;
        }
    }
    
    return 1;
}

int main(int argc, char** argv) {
    char input[] = "tanajura";
    Node root;
    root.tamanho_nodes_internos = 1;
    *root.nodes_internos = calloc(1, sizeof(Node));
    insere_string(input, 0, &root);
    printf("%c\n", root.nodes_internos[0]->caractere);
    return (EXIT_SUCCESS);
}

