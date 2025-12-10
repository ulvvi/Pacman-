//player.h
#pragma once
#include <math.h>
#include "../header.h"

/**
 * @brief Verifica se o jogador está centralizado em uma célula do grid.
 * @param pacman Ponteiro para a estrutura do jogador.
 */
bool checaPlayerCentralizado(tJogador *pacman)
{
    return (((int)pacman->pos.x % TAM_GRID) == 0 && ((int)pacman->pos.y % TAM_GRID) == 0);
}


/**
 * @brief Verifica se o jogador está dentro dos limites do mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 */
bool checaPlayerDentroMapa(tJogador *pacman)
{
    return (pacman->pos.x > 0 && pacman->pos.x < TAM_GRID*(TAM_J-1) && pacman->pos.y > 0 && pacman->pos.y < TAM_GRID*(TAM_I-1));
}


/**
 * @brief Centraliza o jogador na posição inicial definida no mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param grid_mapa Matriz 2D representando o mapa do jogo.
 */
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
                pacman->colisao_player.x = pacman->pos.x + (TAM_GRID - pacman->colisao_player.width)/2;;
                pacman->colisao_player.y = pacman->pos.y + (TAM_GRID - pacman->colisao_player.height)/2;;
                pacman->move_x = 0;
                pacman->move_y = 0;
                pacman->spd = 2;
                pacman->spritesheet.y = 0;
                pacman->comendo.spritesheet.y = 0;
                pacman->comendo.tempo_frame = 0.060;
                pacman->curFruitTimer = 0;
                pacman->current_fruit = -1;
                pacman->fruta_ativa = -1;
                break;
               
            }
        }
    }
}

/**
 * @brief Função que inicializa o jogador com valores padrão.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param pellets Número total de pellets no mapa.
 * @param assets Estrutura contendo as texturas do jogo.
 */
void inicializaPlayer(tJogador* pacman, int pellets, tAssets assets)
{
    //colisao
    pacman->colisao_player.height = 30;
    pacman->colisao_player.width= 30;
    //movimentacao
    pacman->dir = 1;
    pacman->move_x = 0;
    pacman-> move_y = 0;
    pacman->spd = 2;
    //coisas gerais
    pacman->power_pellet = false;
    pacman->fruta_ativa = -1;
    pacman->score = 0;
    pacman->vida = 3; 
    pacman->remainingPellets = pellets;
    pacman->desenho = true;
    pacman->tempo_power_pellet = 0;
    pacman->current_fruit = -1;
    pacman->curFruitTimer = 0;

    //sprite base pacman
    pacman->sprite = assets.pacman_sprite;
    pacman->spritesheet.height = 40;
    pacman->spritesheet.width = 40;
    pacman->spritesheet.x = 0;
    pacman->spritesheet.y = 0;

    //animacao comendo
    pacman->comendo.frame_atual = 0;
    pacman->comendo.total_frames = 4;
    pacman->comendo.tempo_frame = 0.045;
    pacman->comendo.contador = 0;
    pacman->comendo.sprite = assets.pacman_comendo_sprite;
    pacman->comendo.spritesheet.height = TAM_GRID;
    pacman->comendo.spritesheet.width = TAM_GRID;
    pacman->comendo.spritesheet.x = 0;
    pacman->comendo.spritesheet.y = 0;
    pacman->comendo.pos.x = 0;
    pacman->comendo.pos.y = 0;
    pacman->comendo.escala = 1;

    //animacao morte
    pacman->cutscene_morte.frame_atual = 0;
    pacman->cutscene_morte.total_frames = 20;
    pacman->cutscene_morte.tempo_frame = 0.100;
    pacman->cutscene_morte.contador = 0;
    pacman->cutscene_morte.sprite = assets.pacman_cutscene_morte_sprite;
    pacman->cutscene_morte.spritesheet.height = TAM_GRID;
    pacman->cutscene_morte.spritesheet.width = TAM_GRID;
    pacman->cutscene_morte.spritesheet.x = 0;
    pacman->cutscene_morte.spritesheet.y = 0;
    pacman->cutscene_morte.coluna_atual = 0;
    pacman->cutscene_morte.linha_atual = 0;
    pacman->cutscene_morte.escala = 1;
        //a pos do objeto cutscene n precisa inicializar, pra ela tanto faz, vai ser atualizada por frame
    pacman->cutscene_morte.rotacao = 0.0;
}

/**
 * @brief Função para trocar o sprite do Pacman com base na direção e fruta ativa.
 * @param pacman Ponteiro para a estrutura do jogador.
 */ 
void trocaSpritePacman(tJogador* pacman)
{
    pacman->cutscene_morte.spritesheet.width = abs(pacman->cutscene_morte.spritesheet.width);
    pacman->comendo.spritesheet.width = abs(pacman->comendo.spritesheet.width);
    pacman->cutscene_morte.rotacao = 0;
    pacman->comendo.rotacao = 0;
    switch(pacman->fruta_ativa)
    {
        case GRAPE:
            pacman->comendo.tempo_frame = 0.100;
        break;
        case STRAWBERRY:
        break;

        case BLUEBERRY:
            pacman->comendo.tempo_frame = 0.090;
        break;

        case CHERRY:
            pacman->comendo.tempo_frame = 0.120;
        break;

        case -1:
            pacman->comendo.tempo_frame = 0.060;
        break;
        
        pacman->comendo.spritesheet.y = 40*pacman->fruta_ativa;
    }
    
    //mudanca de sprite dependendo da fruta ativa
    if(pacman->fruta_ativa != -1)
    {
        pacman->comendo.spritesheet.y = 40 + 40*pacman->fruta_ativa;
        pacman->spritesheet.y = 40 + 40*pacman->fruta_ativa;
    }else{pacman->comendo.spritesheet.y = 0; pacman->spritesheet.y = 0;}
    

    if(pacman->move_x > 0)
    {
        pacman->spritesheet.x = 0;

    }
    if(pacman->move_x < 0)
    {
        pacman->spritesheet.x = 40;
        pacman->comendo.spritesheet.width = -pacman->comendo.spritesheet.width;
        pacman->cutscene_morte.spritesheet.width = -pacman->cutscene_morte.spritesheet.width;
    }
    if(pacman->move_y < 0)
    {
        pacman->comendo.rotacao = 270;
        pacman->cutscene_morte.rotacao = 270;
        pacman->spritesheet.x = 120;
    }
    if(pacman->move_y > 0)
    {
        pacman->comendo.rotacao = 90;
        pacman->cutscene_morte.rotacao = 90;
        pacman->spritesheet.x = 80;
    }

}

/**
 * @brief Verifica colisões entre o jogador e os pellets no mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param grid_mapa Matriz 2D representando o mapa do jogo.
 * @param score Ponteiro para a pontuação do jogador.
 * @param totalPellets Ponteiro para o número total de pellets restantes.
 * @param state Ponteiro para o estado atual do jogo.
 * @param pontuacao Ponteiro para a estrutura de efeitos visuais.
 * @param gameSFX Array de sons do jogo.
 */
void colisaoPellets(tJogador* pacman, char** grid_mapa, int* score, int* totalPellets, GameState* state, tVfx* pontuacao, Sound gameSFX[8])
{
    //grid atual
    int grid_i = pacman->pos.y/TAM_GRID;
    int grid_j = pacman->pos.x/TAM_GRID;
    int pontuou = 0;

    switch(grid_mapa[grid_i][grid_j])
    {
    
    //pellet
    case '.':
        pontuou = 10;
        grid_mapa[grid_i][grid_j] = ' ';
        (*totalPellets)--;
    break;
    //power pellet
    case 'o':
        pontuou = 50;
        if(pacman->power_pellet == false) *state = CUT_IN;
        pacman->power_pellet = true;
        grid_mapa[grid_i][grid_j] = ' ';
        (*totalPellets)--;
        pacman->tempo_power_pellet+= 8;
    break;

        //frutas
    
        case 'C':
            pontuou = 300;
            getFruit(pacman, 'C');
            PlaySound(gameSFX[7]);
            grid_mapa[grid_i][grid_j] = ' ';
        break;

        case 'S':
            pontuou = 300;
            getFruit(pacman, 'S');
            PlaySound(gameSFX[7]);
            grid_mapa[grid_i][grid_j] = ' ';
        break;

        case 'G':
            pontuou = 300;
            getFruit(pacman, 'G');
            PlaySound(gameSFX[7]);
            grid_mapa[grid_i][grid_j] = ' ';
        break;

        case 'B':
            pontuou = 300;
            getFruit(pacman, 'B');
            PlaySound(gameSFX[7]);
            grid_mapa[grid_i][grid_j] = ' ';
        break;
    }
    (*score)+= pontuou;
    if(pontuou > 10)
    {
        pontuacao->ativo = true;
        pontuacao->parametro_especial = pontuou;
    }
}

/**
 * @brief Função que gerencia o tempo do power pellet.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param mapa Ponteiro para a estrutura do mapa.
 */
void powerPellet(tJogador* pacman, tMapa* mapa)
{   
    pacman->tempo_power_pellet -= GetFrameTime();
    if(pacman->tempo_power_pellet <= 0)
    {
        trocaCorEXT(mapa, mapa->cor_atual);
        pacman->power_pellet = false;
    }
}

/**
 * @brief Atualiza a área de colisão do jogador com base em sua posição.
 * @param pacman Ponteiro para a estrutura do jogador.
 */
void atualizaColisaoPlayer(tJogador* pacman)
{
    pacman->colisao_player.x = pacman->pos.x + (TAM_GRID - pacman->colisao_player.width)/2;
    pacman->colisao_player.y = pacman->pos.y + (TAM_GRID - pacman->colisao_player.height)/2;
}

/**
 * @brief Função que move o jogador com base na entrada do usuário e na colisão com o mapa.
 * @param grid_mapa Matriz 2D representando o mapa do jogo.
 * @param pacman Ponteiro para a estrutura do jogador.
 */
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
        move_alvo_x = IsKeyPressed(KEY_RIGHT) - IsKeyPressed(KEY_LEFT);
        move_alvo_y = 0;
    }
    //input vertical
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
    {
        move_alvo_y = IsKeyPressed(KEY_DOWN) - IsKeyPressed(KEY_UP);
        move_alvo_x = 0;
    }
    
    //inversao imediata de posicao(no msm eixo)
    if(pacman->move_x != 0 && abs(pacman->move_x) == abs(move_alvo_x*pacman->spd))
    {
        pacman->move_x = move_alvo_x*pacman->spd;
        pacman->dir = move_alvo_x;
    }
    if(pacman->move_y != 0 && abs(pacman->move_y) == abs(move_alvo_y*pacman->spd))
    {
        pacman->move_y = move_alvo_y*pacman->spd;
        pacman->dir = move_alvo_y;
    }
    //troca de eixo
    if(checaPlayerCentralizado(pacman) && checaPlayerDentroMapa(pacman))
    {
        //calculo grid atual
        int grid_i = pacman->pos.y/TAM_GRID;
        int grid_j = pacman->pos.x/TAM_GRID;

        if(pacman->fruta_ativa != GRAPE){
            if(move_alvo_y != 0 && grid_mapa[grid_i+move_alvo_y][grid_j] != '#')
            {
                pacman->move_y = move_alvo_y*pacman->spd;
                pacman->dir = move_alvo_y;
                pacman->move_x = 0;
                move_alvo_y = 0;
                
            
            }
            else if(move_alvo_x != 0 && grid_mapa[grid_i][grid_j+move_alvo_x] != '#')
            {
                pacman->move_x = move_alvo_x*pacman->spd;
                pacman->dir = move_alvo_x;
                pacman->move_y = 0;
                move_alvo_x = 0;
            }
            //caso em que nao se aperta pra virar em uma intersecao(continua no msm eixo andando reto)
            if(grid_mapa[(grid_i+pacman->move_y/pacman->spd)][grid_j+(pacman->move_x/pacman->spd)] == '#')
            {
                pacman->move_x = 0;
                pacman->move_y = 0;
            }
        } else {

            if(move_alvo_y != 0)
            {
                pacman->move_y = move_alvo_y*pacman->spd;
                pacman->dir = move_alvo_y;
                pacman->move_x = 0;
                move_alvo_y = 0;
                
            
            }
            else if(move_alvo_x != 0)
            {
                pacman->move_x = move_alvo_x*pacman->spd;
                pacman->dir = move_alvo_x;
                pacman->move_y = 0;
                move_alvo_x = 0;
            }
        }
    }
    //att da pos
    pacman->pos.x += pacman->move_x;
    pacman->pos.y += pacman->move_y;

    trocaSpritePacman(pacman);
    atualizaColisaoPlayer(pacman);
}


/**
 * @brief Função para teletransportar o jogador quando sai dos limites do mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 */
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


