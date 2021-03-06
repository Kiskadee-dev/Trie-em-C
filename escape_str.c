/* 
 * File:   escape_str.h
 * Author: matheus
 *
 * escape_str(char* str) Sanitiza as palavras para que estejam no formato correto especificado.
 * Se a string for apenas uma pontuação, retorna uma string de tamanho 0 (vazia)
 * 
 * ehnumero(char* str) Retorna se a string forma apenas um número, no caso -3.14 é verdade
 * 
 * Created on 28 de Junho de 2019, 12:22
*/

#include <ctype.h>
#include <string.h>

int ehnumero(char* str);

char* escape_str(char* string){
    int start = 0; //Marca o início da string
    int size = strlen(string);
    if(!ehnumero(string)){
        for(int i = 0; i < size; i++){
            if(!isalpha(string[i])){ //Se não for uma letra
                int delete_char = 1;
                for(int j = i; j < size; j++){ //Procura se tem letras adiante
                    if(isalpha(string[j]) || isdigit(string[j])){ //Se tiver uma letra adiante ou um número, não é o final da string
                        if(start == i && !isdigit(string[j])){ //Mas se for o início, remove a pontuação passando o ponteiro adiante
                                start++;
                        }
                        delete_char=0; //Se não for o final, o caractere está OK.
                        break;
                    }
                }
                if(delete_char){ //Se for o final e só tiver pontuações adiante, a string termina aqui.
                    string[i] = '\0';
                    break;
                }
            }else
                string[i] = tolower(string[i]); //Passa tudo para minusculo
        }
    }
    string = &string[start];
    return string;
}

int ehnumero(char* str){
    int size = strlen(str);

    for(int i = 0; i < size; i++){
        if(!isdigit(str[i])){
            if(str[i] != '+' && str[i] != '-' && str[i] != '.')
                return 0;
        }
    }
    /*
    for(int i = 0; i < size-1; i++){ //Se o final da palavra for uma ',' por exemplo, ainda é um número
        if(!isdigit(str[i])){
            return 0;
        }
    }
     */
    return 1;
}

