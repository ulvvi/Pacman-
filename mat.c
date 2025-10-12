#include <stdio.h>
#include "../../../msys64/ucrt64/include/raylib.h"
#include <stdlib.h>
#include <string.h>
#define LINHA 20
#define COLUNA 40



void createMap(FILE *map, char mapa[][COLUNA]){
    char filename[99];
    char ext[] = ".txt";
    
    //nome do arquivo para guardar o mapa
    puts("coloque o nome do arquivo: ");
    scanf("%[^\n]", filename);
    strcat(filename, ext);
    map = fopen(filename, "r");
    if(map == NULL){
        puts("erro de abertura");
        exit(1);
    }

    //coloca as informação do arquivo para uma matriz que será o mapa;
    char cur;
    int a = 0;
    int b = 0;
    while((cur = fgetc(map)) != EOF){
        if(cur == '\n'){
            a++;
            b = 0;
        } else {
            mapa[a][b] = cur;
            b++;
        }
        
    }

    fclose(map);

    for (int i = 0; i < LINHA; i++){
        for(int j = 0; j < COLUNA; j++){
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }



}


int main(){
    FILE *map;
    char filename[99];
    char ext[] = ".txt";
    

    //alocação da matriz do mapa
    char** mapa = (char**) malloc(sizeof(char*) * LINHA);
    if(mapa == NULL){
        puts("erro");
        return -1;
    }
    for(int i = 0; i < LINHA; i++){
        *(mapa + i) = (char*) malloc(sizeof(char) * COLUNA);
        if(*(mapa + i) == NULL){
            puts("erro");
            return -1;
        }
    }

    createMap(map, mapa);

    return 0;
}