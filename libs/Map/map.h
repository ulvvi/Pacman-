//map.h
#pragma once
#include "../header.h"

Color PEL = {255, 213, 65, 255};


/**
 * @brief Função que modifica o nome do arquivo do mapa com base no nível.
 * @param filename Vetor de caracteres para armazenar o nome do arquivo.
 */
void modificaFilename(char filename[], int level)
{
    sprintf(filename, "maps/mapa%d.txt", level);
}

/**
 * @brief Função que aloca dinamicamente a matriz do mapa.
 * @return Ponteiro para a matriz alocada.
 */
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

/**
 * @brief Função que inicializa o mapa a partir de um arquivo.
 * @param filename Nome do arquivo do mapa.
 * @param mapa Matriz para armazenar o mapa.
 * @return Número total de pellets no mapa.
 */
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

/**
 * @brief Função que desenha o mapa na tela.
 * @param mapa Estrutura do mapa contendo a matriz e texturas.
 */
void drawMap(tMapa mapa) 
{
    for(int i = 0; i < TAM_I; i++) 
    {
        for(int j = 0; j < TAM_J; j++)
        {
            float x = j * TAM_GRID;
            float y = i * TAM_GRID;
            float center_x = j * TAM_GRID + TAM_GRID / 2;
            float center_y = i * TAM_GRID + TAM_GRID / 2;

            switch(mapa.grid_mapa[i][j])
            {
                //desenhar parede
                case '#':
                    //precisa fazer mais nada paizao, fica em paz
                    break;
                //desenhar pellet
                case '.':
                    DrawCircle(center_x, center_y, 5, PEL);

                    break;
                //desenhar super pellet
                case 'o':
                    DrawCircle(center_x, center_y, 10, PEL);

                    break;
                case 'T':
                    //faça nada tambem
                    break;
                //desenhar(ou n kkkkkkkkkkkkk) os vazios
                case ' ':
                    break;
                //desenhar fruta

                //FRUTAS
                case 'C':
                    DrawTexture(mapa.cherry, x, y, WHITE);
                    break;
                case 'S':
                    DrawTexture(mapa.strawberry, x, y, WHITE);
                    break;
                case 'G':
                    DrawTexture(mapa.grape, x, y, WHITE);
                    break;
                case 'B':
                    DrawTexture(mapa.blueberry, x, y, WHITE);
                    break;

                //bizzarrices
                default:
                    break;
            }
        }
    }
}

/**
 * @brief Função que libera a memória alocada para o mapa.
 * @param grid_mapa Matriz do mapa a ser liberada.
 */
void freeMap(char** grid_mapa)
{
    //liberar memoria alocada dinamicamente
    for(int i = 0; i < TAM_I; i++)
    {
        free(grid_mapa[i]);
    }
    free(grid_mapa);
    return;
}

/**
 * @brief Função que inicializa a matriz auxiliar usada em algoritmos de pathfinding.
 * @return Ponteiro para a matriz auxiliar inicializada.
 */
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

/**
 * @brief Função que libera a memória alocada para a matriz auxiliar.
 * @param matriz_auxiliar Matriz auxiliar a ser liberada.
 */
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

/**
 * @brief Função que faz o spawn de uma fruta no mapa com uma certa probabilidade.
 * @param gridMapa Matriz do mapa onde a fruta será spawnada.
 */
void fruitSpawn(char** gridMapa){
    int spawnChance = GetRandomValue(1, 10000);
    if(spawnChance <= 5)
    {
        int x, y;
        while(gridMapa[y][x] != ' '){
            x = GetRandomValue(1, TAM_J - 2);
            y = GetRandomValue(1, TAM_I - 2);
        }

        gridMapa[y][x] = 'U';
    }
}