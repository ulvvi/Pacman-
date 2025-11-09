//map.h
#pragma once
#include "../header.h"

//inicializa a matriz com o nome do arq
char** allocateMap(){
    char** grid_mapa = (char**)malloc(sizeof(char*)*TAM_I);
    if(grid_mapa == NULL)
        return NULL;

    for(int i = 0; i < TAM_I; i++)
    {
        *(grid_mapa+i) = (char*)malloc(sizeof(char)*TAM_J);
        if(*(grid_mapa+i) == NULL)
            return NULL;
    }
    return grid_mapa;
}

//inicializa a matriz com o nome do arq
int initMap(char filename[], char **mapa) {
    ///abrir
    FILE *map;
    int p = 0; //contador de pellets
    map = fopen(filename, "r");
    if(map == NULL)
    {
        puts("erro de abertura");
        exit(1);
    }
    //coloca as informação do arquivo para uma matriz que será o mapa;
    char cur;
    int a = 0;
    int b = 0;
    while((cur = fgetc(map)) != EOF) 
    {
        if(cur == '\n') 
        {
            a++;
            b = 0;
        } 
        else 
        {
            mapa[a][b] = cur;
            b++;
        }
        if(cur == '.' || cur == 'o') 
        {
            p++;//conta os pellets
        } 
    }
    ///fechar
    fclose(map);
    return p;  
}

//desenha o mapa com a matriz
void drawMap(char** mapa) 
{
    for(int i = 0; i < TAM_I; i++) 
    {
        for(int j = 0; j < TAM_J; j++)
        {
            float x = j * TAM_GRID;
            float y = i * TAM_GRID;
            float center_x = j * TAM_GRID + TAM_GRID / 2;
            float center_y = i * TAM_GRID + TAM_GRID / 2;

            switch(mapa[i][j])
            {
                //desenhar parede
                case '#':
                    //precisa fazer mais nada paizao, fica em paz
                    break;
                //desenhar pellet
                case '.':
                    DrawCircle(center_x, center_y, 5, ORANGE);

                    break;
                //desenhar super pellet
                case 'o':
                    DrawCircle(center_x, center_y, 10, ORANGE);

                    break;
                case 'T':
                    //faça nada tambem
                    break;
                //desenhar(ou n kkkkkkkkkkkkk) os vazios
                case ' ':
                    break;
                //bizzarrices
                default:
                    break;
            }
        }
    }
}

void freeDiddy(char** grid_mapa)
{
    //liberar memoria alocada dinamicamente
    for(int i = 0; i < TAM_I; i++)
    {
        free(grid_mapa[i]);
    }
    free(grid_mapa);
    return;
}

int** inicializaMatrizAux(){
    int** matriz_auxiliar = (int**)malloc(sizeof(int*)*TAM_I);
    if(matriz_auxiliar == NULL)
        return NULL;

    for(int i = 0; i < TAM_I; i++)
    {
        *(matriz_auxiliar+i) = (int*)malloc(sizeof(int)*TAM_J);
        if(*(matriz_auxiliar+i) == NULL)
            return NULL;
    }
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            matriz_auxiliar[i][j] = -1;
        }
    }
    return matriz_auxiliar;
}



void freeMatrizAux(int** matriz_auxiliar)
{
    //liberar memoria alocada dinamicamente
    for(int i = 0; i < TAM_I; i++)
    {
        free(matriz_auxiliar[i]);
    }
    free(matriz_auxiliar);
    return;
}

