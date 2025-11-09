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
                break;
            }
        }
    }
}


/*COLISAO COM PELLETS(ATUALIZA SCORE E ESTADO AO PEGAR POWER PELLET)*/
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

/*CRONOMETRA O ESTADO E ATT A SPD E ATT O GAMESTATE*/
void powerPellet(tJogador* pacman, GameState* game_state)
{  
    static int tempo_restante = 460;
    static int cheque = 0;
    if(cheque == 0)
    {
        pacman->spd = 4;
        *game_state = CUT_IN;
        cheque = 1;
    }

    tempo_restante--;
    if(tempo_restante <= 0)
    {
        pacman->power_pellet = false;
        pacman->spd = 2;
        cheque = 0;
        tempo_restante = 460;
    }
}
/*MOVIMENTACAO GERAL DO PLAYER, ATUALIZA SUA POSICAO*/
void movePlayer(char** grid_mapa, tJogador* pacman, int* grid_i, int* grid_j)
{
    static bool intencao_horizontal = false, intencao_vertical = false;
    static int move_alvo_x = 0, move_alvo_y = 0;
    bool reverteu = false, virou = false;
    //CORRECAO PRA MOMENTOS EM QUE SE ATUALIZA A SPD DO PACMAN E ELE AINDA TA SE MOVENDO
    //ANTES, AO ATUALIZAR A SPD DELE EM OUTRAS FUNCOES, SO ATUALIZAVA O MOVE_X SE HOUVESSE INPUT
    //SOLUCAO FEIA, MAS RESOLVE. basicamente, é um sincronizador da velocidade, vai ser bem util mais a frente
    if (pacman->move_x > 0) 
        pacman->move_x = pacman->spd;
    if (pacman->move_x < 0) 
        pacman->move_x = -pacman->spd;
    if (pacman->move_y > 0)
         pacman->move_y = pacman->spd;
    if (pacman->move_y < 0) 
        pacman->move_y = -pacman->spd;
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


/*CRIACAO DE RETANGULOS DE COLISAO FANTASMA*/
void criaColisaoFantasma(Rectangle* colisao_fantasma, int n)
{
    colisao_fantasma = malloc(sizeof(Rectangle)*n);
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].height = TAM_GRID;
        colisao_fantasma[i].width = TAM_GRID;
    }
}


/*ATUALIZA COLISAO FANTASMA*/
void atualizaColisaoFantasma(tInimigo* fantasma, Rectangle* colisao_fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].x = fantasma[i].pos.x;
        colisao_fantasma[i].y = fantasma[i].pos.y;
    }
}

/*CHECA COLISAO ENTRE O PLAYER E O FINAL, RETORNA O INDICE DO FANTASMA QUE FOI COLIDIDO, OU 0 SE NAO HOUVE COLISAO*/
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


/*SUBTRAI A VIDA DO JOGADOR E, SE NECESSARIO, DA GAMEOVER*/
void ConcretizaColisao(tJogador* pacman, tInimigo inimigo, int n, Vector2* pos_inicial, char **grid_mapa, int indice)
{
    //checacolisaofantasma na main
    switch(pacman->power_pellet)
    {
        case true:
        
        break;

        case false:
            if(pacman->vida > 0)
            {
                pacman->vida--;
                centralizaPlayer(pacman, grid_mapa);
            }
            else
            {
                gameOver();
            }
        break;
    }
}
