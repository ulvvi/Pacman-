//inclusões
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//macros
#define LARGURA 1600
#define ALTURA 840
#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40
typedef struct
{
    Vector2 pos;
    int spd;
    int vida;
    bool power_pellet;
} tJogador;
typedef struct
{
    //vai ter q criar um array dessa struct se pa na main, dinamicamente
    //vector2 tem os campos x e y
    Vector2 pos;
    int spd;
    bool vulneravel;
} tInimigo;

/*----------------------- FUNCOES -----------------------*/

//inicializa a matriz com o nome do arq
int initMatrix(char filename[], char **mapa) {
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
                    DrawRectangle(x, y, 40, 40, BLUE);

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
                    DrawRectangle(x, y, 40, 40, PURPLE);
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

void centralizaPlayer(tJogador* pacman, char** grid_mapa)
{   
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(grid_mapa[i][j] == 'P')
            {
                pacman->pos.x = j*TAM_GRID;
                pacman->pos.y = i*TAM_GRID;
                break;
            }
        }
    }
}

void teleportaPlayer(tJogador* pacman, int move_x, int move_y)
{
    if(move_x > 0)
    {
        pacman->pos.x = move_x - TAM_GRID;
    }    
    else if(move_x < 0)
    {
        pacman->pos.x = TAM_GRID*(TAM_J) + move_x;
    }

    if(move_y < 0)
    {
        pacman->pos.y = TAM_GRID*(TAM_I) + move_y;
    }
    else if(move_y > 0)
    {
        pacman->pos.y = move_y - TAM_GRID;
    }    
}


//FANTASMAS E PLAYER
void criaColisaoFantasma(Rectangle* colisao_fantasma, int n)
{
    colisao_fantasma = malloc(sizeof(Rectangle)*n);
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].height = TAM_GRID;
        colisao_fantasma[i].width = TAM_GRID;
    }
}
void atualizaColisaoFantasma(tInimigo* fantasma, Rectangle* colisao_fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].x = fantasma[i].pos.x;
        colisao_fantasma[i].y = fantasma[i].pos.y;
    }
}
int checaColisaoFantasma(Rectangle colisao_player, Rectangle* colisao_fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(CheckCollisionRecs(colisao_player, colisao_fantasma[i]))
        {
            return i;
        }
    }
    return 0;
}

void perdeVida(tJogador* pacman, tInimigo inimigo, int n)
{
    if(checaColisaoFantasma != 0)
    {
        pacman->vida--;
        //ativar algum estado de game over ou de reposicionar o pacman no grid
    }
}
