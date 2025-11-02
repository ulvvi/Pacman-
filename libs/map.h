//map.h
#pragma once
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "system.h"

//macros
#define LARGURA 1600
#define ALTURA 840
#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40

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

/*
a logica por tras dessa funcao é relativamente simples: ela usa bastante a ideia de 'sobreposicao'. inicialmente, todos os quadrados com paredes
sao inicializados usando os tiles horizontais(com algumas excecoes). depois, ela 'conserta' os tiles verticais, reescrevendo os valores de matriz_auxiliar 
apenas para os que precisam ser ajeitados, e, por fim, 'conserta' os valores de tile para os demais. sao 12 tiles no total. nao sei se é a abordagem mais 
inteligente, mas na minha cabeca foi uma das q mais fez sentido, afinal, os tiles mais utilizados sao os completamente horizontais e os completamente verticais,
e o restante sao casos excepcionais(apesar de nao serem raros), por isso os tratei de forma separada
*/
void texturaMapa(char **matriz, int**matriz_auxiliar)
{
    //PREENCHIMENTO HORIZONTAL
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(matriz[i][j] == '#')
            {
                tCordenada grid_parede = {{i,j},{i,j} };
                while(matriz[i][j] == '#' && j < TAM_J)
                {
                    matriz_auxiliar[i][j] = MEIO_FECHADO;
                    //correcoes mais para a borda, caso haja tile em baixo ou em cima, n da pra ser o meio fechado ne
                    if(i < TAM_I-1 && matriz[i+1][j] == '#')   
                    {
                        matriz_auxiliar[i][j] = MEIO_ABERTO_BAIXO;
                    }
                    else if(i > 0 && matriz[i-1][j] == '#')
                    {
                        matriz_auxiliar[i][j] = MEIO_ABERTO_CIMA;
                    }
                    j++;
                }
                //consertar sobrecontagem
                j--;
                //correcoes esquerda e direita
                grid_parede.grid_final[1] = j;
                matriz_auxiliar[grid_parede.grid_inicio[0]][grid_parede.grid_inicio[1]] = FIM_ESQUERDA;
                matriz_auxiliar[grid_parede.grid_final[0]][grid_parede.grid_final[1]] = FIM_DIREITA;
            }
        }
    }

    //preenchimento bordas verticais
    for(int j = 0; j < TAM_J; j+=TAM_J-1)
    {
        for(int i = 1; i < TAM_I-1; i++)
        {
            if(matriz[i][j] == '#')
            {
                matriz_auxiliar[i][j] = CIMA_ABERTO;
                //mesma ideia do if la no horizontal, correcao caso as bordas verticais, especificamente, tenham tiles grudados em sua direita ou esquerda
                if(matriz[i][j+1] == '#')
                {
                    matriz_auxiliar[i][j] = CIMA_ABERTO_DIREITA;
                }
                else if(j > 1 && matriz[i][j-1] == '#') 
                {
                    matriz_auxiliar[i][j] = CIMA_ABERTO_ESQUERDA;
                }
            }
        }
    }
    //preenchimento diagonais das bordas
    matriz_auxiliar[0][0] = CIMA_PARA_DIREITA;
    matriz_auxiliar[0][TAM_J-1] = CIMA_PARA_ESQUERDA;
    matriz_auxiliar[TAM_I-1][0] = ESQUERDA_PARA_CIMA;
    matriz_auxiliar[TAM_I-1][TAM_J-1] = DIREITA_PARA_CIMA;

    //preenchimento das paredes verticais no geral
    for(int j = 1; j < TAM_J-1; j++)
    {
        for(int i = 1; i < TAM_I-1; i++)
        {
            if(matriz[i][j] == '#' && matriz[i][j+1] != '#' && matriz[i][j-1] != '#')
            {
                tCordenada grid_parede = {{i,j},{i,j}};
                while(matriz[i][j] == '#' && matriz[i][j+1] != '#' && matriz[i][j-1] != '#' && i < TAM_I)
                {
                    matriz_auxiliar[i][j] = CIMA_ABERTO;
                    i++;
                }
                //consertar a sobrecontagem
                i--;
                //correcoes fechado/aberto
                grid_parede.grid_final[0] = i;
                if(matriz[grid_parede.grid_inicio[0] - 1][grid_parede.grid_inicio[1]] != '#')
                    matriz_auxiliar[grid_parede.grid_inicio[0]][grid_parede.grid_inicio[1]] = CIMA_FECHADO;
                if(matriz[grid_parede.grid_final[0] + 1][grid_parede.grid_final[1]] != '#')
                    matriz_auxiliar[grid_parede.grid_final[0]][grid_parede.grid_final[1]] = MEIO_FECHADO_CIMA;

            }
        }
    }
    //CORRECAO DO RESTO(casos excepcionais que envolvem tiles nao basicos)
    for(int j = 1; j < TAM_J-1; j++)
    {
        for(int i = 1; i < TAM_I-1; i++)
        {
            if(matriz[i][j] == '#')
            {
                //pouca bool gracas a deus(tile ate em Tupi)
                bool meio_aberto_baixo = (matriz[i+1][j] == '#' && matriz[i-1][j] != '#' && matriz[i][j+1] == '#' && matriz[i][j-1] == '#'); 
                bool meio_aberto_cima = (matriz[i+1][j] != '#' && matriz[i-1][j] == '#' && matriz[i][j+1] == '#' && matriz[i][j-1] == '#');
                bool esquerda_para_cima = (matriz[i+1][j] != '#' && matriz[i-1][j] == '#' && matriz[i][j+1] == '#' && matriz[i][j-1] != '#');
                bool cima_para_direita = (matriz[i][j+1] == '#' && matriz[i+1][j] == '#' && matriz[i][j-1] != '#' && matriz[i-1][j] != '#');
                bool cima_para_esquerda = (matriz[i][j-1] == '#' && matriz[i+1][j] == '#' && matriz[i][j+1] != '#' && matriz[i-1][j] != '#');
                bool direita_para_cima = (matriz[i][j-1] == '#' && matriz[i-1][j] == '#' && matriz[i][j+1] != '#' && matriz[i+1][j]!= '#');
                bool vazio = (matriz[i+1][j] == '#' && matriz[i-1][j] == '#' && matriz[i][j+1] == '#' && matriz[i][j-1] == '#');
                bool cima_aberto_esquerda = (matriz[i+1][j] == '#' && matriz[i-1][j] == '#' && matriz[i][j-1] == '#' && matriz[i][j+i] != '#');
                bool cima_aberto_direita = (matriz[i+1][j] == '#' && matriz[i-1][j] == '#' && matriz[i][j+1] == '#' && matriz[i][j-1] != '#');
                bool bloco_unico = (matriz[i+1][j] != '#' && matriz[i-1][j] != '#' && matriz[i][j+1] != '#' && matriz[i][j-1] != '#');
                //checagem
                if(esquerda_para_cima == true) matriz_auxiliar[i][j] = ESQUERDA_PARA_CIMA;
                if(meio_aberto_cima == true) matriz_auxiliar[i][j] = MEIO_ABERTO_CIMA; 
                if(meio_aberto_baixo == true) matriz_auxiliar[i][j] = MEIO_ABERTO_BAIXO;
                if(cima_para_direita == true) matriz_auxiliar[i][j] = CIMA_PARA_DIREITA;
                if(cima_para_esquerda == true) matriz_auxiliar[i][j] = CIMA_PARA_ESQUERDA;
                if(direita_para_cima == true) matriz_auxiliar[i][j] = DIREITA_PARA_CIMA;
                if(cima_aberto_esquerda == true) matriz_auxiliar[i][j] = CIMA_ABERTO_ESQUERDA;
                if(cima_aberto_direita == true) matriz_auxiliar[i][j] = CIMA_ABERTO_DIREITA;
                if(bloco_unico == true) matriz_auxiliar[i][j] = BLOCO_UNICO;
                if(vazio == true) matriz_auxiliar[i][j] = VAZIO;
            }
        }
    }
}

void drawTexturaParede(int** matriz_auxiliar, Texture2D tileset, Rectangle spritesheet)
{
    Vector2 pos;
    for(int i = 0; i < TAM_I; i++) 
    {
        for(int j = 0; j < TAM_J; j++)
        {
            pos.x = j * TAM_GRID;
            pos.y = i * TAM_GRID;
            if(matriz_auxiliar[i][j] != -1)
            {
                spritesheet.x += 40*(matriz_auxiliar[i][j]);
                DrawTextureRec(tileset, spritesheet, pos, WHITE);
                spritesheet.x = 0;
            }
        }
    }
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