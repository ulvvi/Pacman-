#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#define TAM_I 20
#define TAM_J 40

void initMatrix(char filename[], char **mapa) {
    ///abrir
    FILE *map;
    map = fopen(filename, "r");
    if(map == NULL){
        puts("erro de abertura");
        exit(1);
    }
    //coloca as informação do arquivo para uma matriz que será o mapa;
    char cur;
    int a = 0;
    int b = 0;
    while((cur = fgetc(map)) != EOF) {
        if(cur == '\n') {
            a++;
            b = 0;
        } else {
            mapa[a][b] = cur;
            b++;
        }      
    }
    ///fechar
    fclose(map);
}

int main(){
    ///nome que sera do arquivo
    char nomeArq[] = "level1.txt";
    
    //alocação da matriz do mapa
    char** mapa = (char**) malloc(sizeof(char*) * TAM_I);
    if(mapa == NULL) {
        puts("erro");
        return -1;
    }
    for(int i = 0; i < TAM_I; i++) {
        *(mapa + i) = (char*) malloc(sizeof(char) * TAM_J);
        if(*(mapa + i) == NULL){
            puts("erro");
            return -1;
        }
    }

    createMap(nomeArq, mapa); ///exemplo de chamada

    return 0;
}
