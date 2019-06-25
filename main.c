/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tamanho_nodes_internos = 256;

typedef struct node{
   char caractere;
   int tamanho_nodes_internos;
   struct node* nodes_internos[1000];
   int dados;
   int termina;
   int profundidade;
} Node;

int verbose=0;
int tamanho_root = 1;

int insere_string(char *str, int profundidade, Node* alvo, int dados){
    if(profundidade < strlen(str)){
        if(verbose){
            if(profundidade == 0){
                printf("PALAVRA: [%s]\n", str);
            }
            printf("Inserir [%c], profundidade=[%d]\n", str[profundidade], profundidade);
            
        }
        Node* atual = alvo; //Alvo é o node escolhido, se inicia no root e depois desce até o fim.
        int novo = 1;
        if(atual->nodes_internos[str[profundidade]]->caractere != '\0'){
            if(profundidade < strlen(str)-1){
                if(verbose){
                    printf("Existe [%c]\n", str[profundidade]);
                }
            }else{
               if(!atual->nodes_internos[str[profundidade]]->termina){ //É uma letra existente mas é uma palavra nova
                   atual->nodes_internos[str[profundidade]]->termina = 1;
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
            novo = 0;
            return 0;
        }
            

        if(novo){
            //Escolhe um node vago dentro do node
            if(verbose)
                printf("Inserindo [%c]\n", str[profundidade]);
            atual = atual->nodes_internos[str[profundidade]];
        }
        if(profundidade < strlen(str)-1){ //Não é uma folha
            atual->caractere = str[profundidade];
            atual->termina=0;
            atual->profundidade=profundidade;
            atual->tamanho_nodes_internos=tamanho_nodes_internos;
            for(int i = 0; i < atual->tamanho_nodes_internos; i++){ //Preenche os espaços dentro do node inserido
                Node* n = malloc(sizeof(Node));
                n->caractere = '\0';
                atual->nodes_internos[i] = n;
            }
            insere_string(str, profundidade+1, atual, dados);
        }else{ //se é uma folha então forma uma palavra
            atual->caractere = str[profundidade];
            atual->termina=1;
            atual->dados=dados;
            atual->profundidade = profundidade;
            atual->tamanho_nodes_internos=tamanho_nodes_internos;
            if(verbose)
                printf("Nova palavra! => %d\n", dados);
            for(int i = 0; i < atual->tamanho_nodes_internos; i++){
                Node* n = malloc(sizeof(Node));
                n->caractere = '\0';
                atual->nodes_internos[i] = n;
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
    Node root;
    root.tamanho_nodes_internos = tamanho_nodes_internos;
    for(int i = 0; i < root.tamanho_nodes_internos; i++){
        root.nodes_internos[i] = malloc(sizeof(Node));
    }
   
    char input[1000] = "Ana";
    insere_string(input, 0, &root, 2);
    printf("\n");
    strcpy(input, "Ana Julia");
    insere_string(input, 0, &root, 30);
    strcpy(input, "Ana Belle");
    insere_string(input, 0, &root, 30);
    strcpy(input, "Anu Belle");
    insere_string(input, 0, &root, 30);
    strcpy(input, "Ann sahduascojdajidjdjfhsasd");
    insere_string(input, 0, &root, 150);
    strcpy(input, "Ann sahduascojdajlasdjasdiwjd");
    insere_string(input, 0, &root, 100);
    return (EXIT_SUCCESS);
}

