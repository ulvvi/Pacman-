//player.h
#pragma once
#include "../header.h"

/*RETORNA TRUE OU FALSE SE O PLAYER TIVER CENTRALIZADO OU NAO*/
bool checaPlayerCentralizado(tJogador *pacman)
{
    return (((int)pacman->pos.x % TAM_GRID) == 0 && ((int)pacman->pos.y % TAM_GRID) == 0);
}


/*RETORNA TRUE OU FALSE SE O PLAYER TIVER DENTRO DO MAPA OU NAO*/
bool checaPlayerDentroMapa(tJogador *pacman)
{
    return (pacman->pos.x > 0 && pacman->pos.x < TAM_GRID*(TAM_J-1) && pacman->pos.y > 0 && pacman->pos.y < TAM_GRID*(TAM_I-1));
}


/*CENTRALIZA O PLAYER NO GRID, SO PRECISA SER CHAMADA NO INICIO DA FASE*/
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
                pacman->colisao_player.x = pacman->pos.x;
                pacman->colisao_player.y = pacman->pos.y;
                pacman->move_x = 0;
                pacman->move_y = 0;
                break;
               
            }
        }
    }
}

void inicializaPlayer(tJogador* pacman, int pellets)
{
    pacman->colisao_player.height = 40;
    pacman->colisao_player.width= 40;
    pacman->dir = 1;
    pacman->power_pellet = false;
    pacman->score = 0;
    pacman->spd = 2;
    pacman->vida = 3; 
    pacman->remainingPellets = pellets;
    pacman->move_x = 0;
    pacman-> move_y = 0;
    
}


/*COLISAO COM PELLETS(ATUALIZA SCORE E ESTADO AO PEGAR POWER PELLET)*/
void colisaoPellets(tJogador* pacman, char** grid_mapa, int* score, int* totalPellets)
{
    //grid atual
    int grid_i = pacman->pos.y/TAM_GRID;
    int grid_j = pacman->pos.x/TAM_GRID;

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

/*CRONOMETRA O ESTADO E ATT A SPD E ATT O GAMESTATE*/
void powerPellet(tJogador* pacman, GameState* game_state)
{  
    static int tempo_restante = 460;
    static int cheque = 0;
    if(cheque == 0)
    {
        *game_state = CUT_IN;
        cheque = 1;
    }

    tempo_restante--;
    if(tempo_restante <= 0)
    {
        pacman->power_pellet = false;
        cheque = 0;
        tempo_restante = 460;
    }
}

void atualizaColisaoPlayer(tJogador* pacman)
{
    pacman->colisao_player.x = pacman->pos.x;
    pacman->colisao_player.y = pacman->pos.y;
}

/*MOVIMENTACAO GERAL DO PLAYER, ATUALIZA SUA POSICAO*/
void movePlayer(char** grid_mapa, tJogador* pacman)
{
    //agora essas duas var servem mais pra indicar a dir no eixo horizontal e vertical(podem ser -1,0,1)
    static int move_alvo_x = 0;
    static int move_alvo_y = 0;

    //att da move_x caso haja atualizacao externa da spd
    if(pacman->move_x > 0) pacman->move_x = pacman->spd;
    else if(pacman->move_x < 0) pacman->move_x = -pacman->spd;
    else if(pacman->move_y > 0) pacman->move_y = pacman->spd;
    else if(pacman->move_y < 0) pacman->move_y = -pacman->spd; 
         
    //input horizontal
    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
    {
        pacman->dir = IsKeyPressed(KEY_RIGHT) - IsKeyPressed(KEY_LEFT);
        move_alvo_x = pacman->dir;
        move_alvo_y = 0;
    }
    //input vertical
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
    {
        pacman->dir =  IsKeyPressed(KEY_DOWN) - IsKeyPressed(KEY_UP);
        move_alvo_x = 0;
        move_alvo_y = pacman->dir;
    }
    
    //inversao imediata de posicao(no msm eixo)
    if(abs(pacman->move_x) == abs(move_alvo_x*pacman->spd))
        pacman->move_x = move_alvo_x*pacman->spd;
    if(abs(pacman->move_y) == abs(move_alvo_y*pacman->spd))
        pacman->move_y = move_alvo_y*pacman->spd;
    
    //troca de eixo
    if(checaPlayerCentralizado(pacman) && checaPlayerDentroMapa(pacman))
    {
        //calculo grid atual
        int grid_i = pacman->pos.y/TAM_GRID;
        int grid_j = pacman->pos.x/TAM_GRID;    

        if(move_alvo_y != 0 && grid_mapa[grid_i+move_alvo_y][grid_j] != '#')
        {
            pacman->move_y = move_alvo_y*pacman->spd;
            pacman->move_x = 0;
            move_alvo_y = 0;
            
        }
        else if(move_alvo_x != 0 && grid_mapa[grid_i][grid_j+move_alvo_x] != '#')
        {
            pacman->move_x = move_alvo_x*pacman->spd;
            pacman->move_y = 0;
            move_alvo_x = 0;
        }
        //caso em que nao se aperta pra virar em uma intersecao(continua no msm eixo andando reto)
        if(grid_mapa[(grid_i+pacman->move_y/pacman->spd)][grid_j+(pacman->move_x/pacman->spd)] == '#')
        {
            pacman->move_x = 0;
            pacman->move_y = 0;
        }
    }
    //att da pos
    pacman->pos.x += pacman->move_x;
    pacman->pos.y += pacman->move_y;
    atualizaColisaoPlayer(pacman);
}


/*TELEPORTA O PLAYER DE ACORDO COM A DIRECAO DELE*/
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


