/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 24 de Junho de 2019, 08:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escape_str.h"
#include <ctype.h>

const int tamanho_nodes_internos = 256;

typedef struct node{
   char caractere;
   struct node* nodes_internos[256];
   int dados;
   int termina;
   int profundidade;
   int ID; //A identificação da palavra no vetor
} Node;

int verbose = 0;
char BUFFER[2000]; //Buffer de print da árvore, não espero que ela seja mais profunda que 2000

char palavras_unicas[11234][2000];
int indices_palavras[11234];
int repeticoes_palavras[11234];
int quantidade_palavras=0;
Node* root;

void limpa_arvore(Node* node, int profundidade){ //Desaloca os nodes da memória, incluindo o raiz. depth first
    for(int i = 0; i < tamanho_nodes_internos; i++){
        if(node->nodes_internos[i] != NULL){
            limpa_arvore(node->nodes_internos[i], profundidade+1);
        }
    }
    if(verbose && profundidade == 0){
        printf("Limpo.\n");
        //printf("Limpando: [%c]\n", node->caractere);
    }
    free(node);
}

void mostra_tudo(Node* node, int profundidade, char* buffer){
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

int checa_string(char* str, int profundidade, Node* node){
    if(str[0] == '\0')
        return (EXIT_FAILURE);

    if(str[profundidade+1] != '\0'){
        if(node->nodes_internos[str[profundidade]] != NULL){
            node = node->nodes_internos[str[profundidade]];
            return checa_string(str, profundidade+1, node);
        }else{
            return (EXIT_FAILURE);
        }
    }else if(node->nodes_internos[str[profundidade]] != NULL){
        if(node->nodes_internos[str[profundidade]]->termina){
            return (EXIT_SUCCESS); //Encontrou a palavra
        }else{
            return (EXIT_FAILURE); //Não forma uma palavra
        }
    }else{
        return (EXIT_FAILURE); //Fim da string a verificar
    }
}

int deleta_string(char* str, int profundidade, Node* node){
    if(str[0] == '\0')
        return (EXIT_FAILURE);
    
   if(node->nodes_internos[str[profundidade]] != NULL){ //Se o node interno existir, caminha até o fim da palavra
        if(str[profundidade+1] != '\0'){ //Se não for a última letra da palavra
            deleta_string(str, profundidade+1, node->nodes_internos[str[profundidade]]); //entra no próximo nível
        }else{
            if(node->nodes_internos[str[profundidade]]->termina){ //Se for a última letra e formar uma palavra,
                node->nodes_internos[str[profundidade]]->termina = 0; //remove o marcador de palavra
                return (EXIT_SUCCESS);
            }
        }
   }else{
        return (EXIT_FAILURE); 
   }
}

int insere_string(char* str, int tamanho, int profundidade, Node* alvo, int dados){
    if(str[profundidade] == '\0') //Se a string terminou de maneira inesperada
        return (EXIT_FAILURE);

    /*
     * Realiza a segunda parte da sanitização da palavra,
     * Caso ela tenha alguma pontuação entre letras, ela representa duas palavras
     * Caso seja um número, -3.14 , ele é lido por inteiro
     * '-3.14,' não é um número, ',' não faz parte dos números
     * e não dá para distinguir entre -3.14,2 e -3.14; 2
     * Neste segundo caso será lido como -3; 14 e 2.
     * 
     * A próxima letra quebra a palavra em mais de uma, portanto, esta letra atual é a última     
     * O caso especial apóstrofo ' é usado para abreviar algumas palavras, [it's] por exemplo.
     * só se abrevia a palavra caso o apóstrofo seja posto antes da última letra.
     * casos como [we're :: we are] são ignorados pois também forma uma palavra normal [were]
     * 
     * hífens são lidos normalmente, couve-flor é uma palavra só
    */

    if(!ehnumero(str) && (str[profundidade+1] != "'"[0] && str[profundidade+2] != '\0') && str[profundidade+1] != '-'){
        if(!isdigit(str[profundidade+1]) && !isalpha(str[profundidade+1])){
            str[profundidade+1] = '\0';
        }
    }
    

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
            insere_string(str, tamanho, profundidade+1, atual->nodes_internos[str[profundidade]], dados); //O node atual existe, insere o próximo dentro dele
            return (EXIT_SUCCESS);
        }else{
           if(!atual->nodes_internos[str[profundidade]]->termina){ //É uma letra existente mas é uma folha, portanto uma palavra nova
               atual->nodes_internos[str[profundidade]]->termina = 1;
               atual->nodes_internos[str[profundidade]]->dados = dados;

                if(verbose){
                   printf("Existe [%c], mas eh uma palavra nova\n", str[profundidade]);
                }
               ///// Insere a palavra no vetor de repetições como única
               strcpy(palavras_unicas[quantidade_palavras], str);
               indices_palavras[quantidade_palavras] = quantidade_palavras;
               atual->nodes_internos[str[profundidade]]->ID = quantidade_palavras;
               repeticoes_palavras[quantidade_palavras]++;
               quantidade_palavras++;
               
               /***Verifica se tem mais palavras na string***/
                if(profundidade < tamanho-1){ //A palavra atual terminou mas ainda há palavras na string
                    insere_string(&str[profundidade+2], strlen(&str[profundidade+2]), 0, root, dados); //Insere a string além do \0, é uma palavra nova
                }
            }else{
                if(verbose){
                   printf("[%c] faz parte da palavra\n", str[profundidade]);
                }
                //Aumenta o número de repetições desta palavra
                repeticoes_palavras[atual->nodes_internos[str[profundidade]]->ID]++;
                
                /***Verifica se tem mais palavras na string***/
                if(profundidade < tamanho-1){ //A palavra atual terminou mas ainda há palavras na string
                    insere_string(&str[profundidade+2], strlen(&str[profundidade+2]), 0, root, dados); //Insere a string além do \0, é uma palavra nova
                }
                return (EXIT_SUCCESS);
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
            insere_string(str, tamanho, profundidade+1, atual, dados); //Desce mais um nível
        }else{ //se é uma folha então forma uma palavra
            atual->termina = 1;
            atual->dados = dados;
            atual->profundidade = profundidade;
            if(verbose)
                printf("Nova palavra! => %d\n", dados);
            ///// Insere a palavra no vetor de repetições como única
            strcpy(palavras_unicas[quantidade_palavras], str);
            indices_palavras[quantidade_palavras] = quantidade_palavras;
            atual->ID = quantidade_palavras;
            repeticoes_palavras[quantidade_palavras]++;

            quantidade_palavras++;
            
            /***Verifica se tem mais palavras na string***/
            if(profundidade < tamanho-1){ //A palavra atual terminou mas ainda há palavras na string
               insere_string(&str[profundidade+2], strlen(&str[profundidade+2]), 0, root, dados); //Insere a string além do \0, é uma palavra nova
           }
        }
    }
    return (EXIT_FAILURE);
}

int sort(const void *x, const void *y){
    const int *a = x, *b = y;
    if(repeticoes_palavras[*a] > repeticoes_palavras[*b]){
        return -1;
    }else if (repeticoes_palavras[*a] < repeticoes_palavras[*b]){
        return 1;
    }else{
        return strcmp(palavras_unicas[*a], palavras_unicas[*b]);
    }
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    
    //Cria o node inicial e aloca seus nodes internos
    root = calloc(1, sizeof(Node));
    char palavra[2000];
    if(input != NULL){
        while(fscanf(input, "%s", palavra) != EOF){ 
            strcpy(palavra, escape_str(palavra)); //Copia a palavra sanitizada para o buffer
            if(strlen(palavra) > 0){ //Se a palavra não for vazia, insere
                insere_string(palavra, strlen(palavra), 0, root, 0);
            }
        }
        //mostra_tudo(root, 0, BUFFER); 
        limpa_arvore(root, 0);
        fclose(input);
    }
    /*
     * Ordena sob o seguinte critério.
     * Palavras com mais repetições vem primeiro.
     * Se uma palavra não tiver repetições, ela é omitida.
     * Caso a palavra esteja empatada em repetições, ela é ordenada em ordem alfabética.
     */
    
    qsort(indices_palavras, quantidade_palavras, sizeof(int), sort);
    
    for(int i = 0; i < quantidade_palavras; i++){
        if(output != NULL){ //Se houver um destino, grava nele, senão, faz o print no stdout
            if(repeticoes_palavras[indices_palavras[i]] > 1){
                fprintf(output, "%s\n", palavras_unicas[indices_palavras[i]]);
            }
        }else{
            if(repeticoes_palavras[indices_palavras[i]] > 1){
                fprintf(stdout, "%s\n", palavras_unicas[indices_palavras[i]]);
            }
        }
    }
    if(output != NULL){
        fclose(output);
    }
    return (EXIT_SUCCESS);
}