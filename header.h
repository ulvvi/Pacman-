//inclusões
//teste
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
    int move_x;
    int move_y;
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

void teleportaPlayer(tJogador* pacman)
{
    if(pacman->move_x > 0)
    {
        pacman->pos.x = pacman->move_x - TAM_GRID;
    }    
    else if(pacman->move_x < 0)
    {
        pacman->pos.x = TAM_GRID*(TAM_J) + pacman->move_x;
    }

    if(pacman->move_y < 0)
    {
        pacman->pos.y = TAM_GRID*(TAM_I) + pacman->move_y;
    }
    else if(pacman->move_y > 0)
    {
        pacman->pos.y = pacman->move_y - TAM_GRID;
    }    
}

//FANTASMAS E PLAYER
void gameOver(void)
{
    int tam_over = 80;
    int tam_resto = 20;
    char texto_over[] = {"FIM DE JOGO"};
    char texto_menu[] = {"V para retornar ao MENU"};
    char texto_sair[] = {"ESC para sair do jogo"};
    bool game_over = true;
    while(game_over == true)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("FIM DE JOGO", (LARGURA - MeasureText(texto_over, tam_over))/2, ALTURA/2 - tam_over/2, 80, RED);
        DrawText("V para retornar ao MENU", (LARGURA - MeasureText(texto_menu, tam_resto))/2, ALTURA/2 + tam_resto*3, tam_resto, YELLOW);
        DrawText("ESC para sair do jogo", (LARGURA - MeasureText(texto_sair, tam_resto))/2, ALTURA/2 + tam_resto*5, tam_resto, YELLOW);
        EndDrawing();

        if(IsKeyPressed(KEY_V))
        {
            //voltar ao menu de alguma forma
            game_over = false;
        }
        if(IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
            exit(0);
        }
    }
}

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

void perdeVida(tJogador* pacman, tInimigo inimigo, int n, Vector2* pos_inicial, char **grid_mapa)
{
    if(checaColisaoFantasma != 0)
    {
        if(pacman->vida > 0)
        {
        pacman->vida--;
        centralizaPlayer(pacman, grid_mapa);
        }
        else
        {
            gameOver();
        }
    }
}

bool checaPlayerCentralizado(tJogador *pacman)
{
    return (((int)pacman->pos.x % TAM_GRID) == 0 && ((int)pacman->pos.y % TAM_GRID) == 0);

}
bool checaPlayerDentroMapa(tJogador *pacman)
{
    return (pacman->pos.x >= 0 && pacman->pos.x <= TAM_GRID*(TAM_J-1) && pacman->pos.y >= 0 && pacman->pos.y <= TAM_GRID*(TAM_I-1));
}

void colisaoPellets(tJogador* pacman, char** grid_mapa, int* score, int* totalPellets, int grid_i, int grid_j)
{
    switch(grid_mapa[grid_i][grid_j])
    {
    //pellet
    case '.':
        (*score)+=10;
        grid_mapa[grid_i][grid_j] = ' ';
        (*totalPellets)--;
    break;
    //power pellet
    case 'o':
        //logica do power pellet(a fazer)
        pacman->power_pellet = true;
        (*score)+=50;
        grid_mapa[grid_i][grid_j] = ' ';
        (*totalPellets)--;
    break; 
    }
}

void movePlayer(char** grid_mapa, tJogador* pacman, int* grid_i, int* grid_j)
{
    static bool intencao_horizontal = false, intencao_vertical = false;
    static int move_alvo_x = 0, move_alvo_y = 0;
    bool reverteu = false, virou = false;
    //pegar o input
    if(IsKeyPressed(KEY_RIGHT))
    {
        intencao_horizontal = true;
        intencao_vertical = false;
        move_alvo_x = pacman->spd;
        move_alvo_y = 0;
    }
    if(IsKeyPressed(KEY_LEFT))
    {
        intencao_horizontal = true;
        intencao_vertical = false;
        move_alvo_x = -pacman->spd;
        move_alvo_y = 0;
    }
    if(IsKeyPressed(KEY_UP))
    {
        intencao_horizontal = false;
        intencao_vertical = true;
        move_alvo_x = 0;
        move_alvo_y = -pacman->spd;
    }
    if(IsKeyPressed(KEY_DOWN))
    {
        intencao_horizontal = false;
        intencao_vertical = true;
        move_alvo_x = 0;
        move_alvo_y = pacman->spd;
    }

    //impedir o delay aparente(se apertar pra se mover no msm eixo, ele n espera centralizar no grid, os dois ifs sao pra isso)
    if(intencao_horizontal == true && move_alvo_x == - pacman->move_x)
    {
        pacman->move_x = move_alvo_x;
        intencao_horizontal = false;
        reverteu = true;
    }
    if(intencao_vertical == true && move_alvo_y == - pacman->move_y)
    {
        pacman->move_y = move_alvo_y;
        intencao_vertical = false;
        reverteu = true;
    }

    if(checaPlayerDentroMapa(pacman) == true)
    {
        //grid atual do player
        *grid_i = (int)pacman->pos.y / TAM_GRID;
        *grid_j = (int)pacman->pos.x / TAM_GRID;

        //tomar a decisao de virar(caso seja necessario)
        if(checaPlayerCentralizado(pacman) == true && reverteu == false)
        {
            //há intencao de mudar de eixo(do vertical pro horizontal ne)
            if(intencao_horizontal == true)
            {
                if((grid_mapa[*grid_i][*grid_j+(move_alvo_x)/pacman->spd]) != '#')
                {
                    pacman->move_x = move_alvo_x;
                    pacman->move_y = 0;
                    intencao_horizontal = false;
                    virou = true;
                }
            }
            //há intencao de mudar de eixo
            else if(intencao_vertical == true)
            {
                if((grid_mapa[*grid_i+(move_alvo_y/pacman->spd)][*grid_j]) != '#')
                {
                    pacman->move_y = move_alvo_y;
                    pacman->move_x = 0;
                    intencao_vertical = false;
                    virou = true;
                }
            }
            //caso de colisao caso ele esteja andando reto em algum eixo
            //tive q adicionar grid_i != 0 na condicao por conta do caso particular em que o jogador ta no grid 0 se movimentando para cima, causando segmentation fault([0 +(-1)][])
            if(virou == false && (*grid_i != 0 && *grid_i != TAM_I-1) && (grid_mapa[*grid_i+(pacman->move_y/pacman->spd)][*grid_j+(pacman->move_x/pacman->spd)]) == '#')
            {
                pacman->move_x = 0;
                pacman->move_y = 0;
            }    
        }
    }
    //atualizacao da pos    
    pacman->pos.x+= pacman->move_x; 
    pacman->pos.y+= pacman->move_y;
    
    //atualizacao do grid apos se mover
    if(checaPlayerDentroMapa(pacman) == true)
    {
        *grid_i = (int)pacman->pos.y / TAM_GRID;
        *grid_j = (int)pacman->pos.x / TAM_GRID;
    }
}