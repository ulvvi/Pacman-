//system.h
#pragma once
#include "../header.h"

//macros
#define LARGURA 1600
#define ALTURA 840
#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40
#define MAP_SIZE (TAM_I * TAM_J)
#define MASK_SIZE ((TAM_I/2) * (TAM_J/2))



typedef struct
{
    Vector2 pos;
    int spd;
    int vida;
    int move_x;
    int move_y;
    bool power_pellet;
    int score;
    int remainingPellets;
    int dir;

    Rectangle colisao_player;
    Rectangle spritesheet;
    Texture2D sprite;

} tJogador;


//STATE MACHINE eba memorias de guerra
typedef enum GameState
{
    MENU,
    GAMEPLAY,
    JACKPOT,
    PAUSE,
    CUT_IN,
    PRIMEIRO_MOVIMENTO

}GameState;

typedef struct 
{
    char** grid_mapa;
    int **matriz_auxiliar;
    int *mapa_mascaras;
    Texture2D tileset_parede; 
    Rectangle spritesheet;
    int tamanho_spritesheet;
    int pellets_totais;

}tMapa;


typedef struct
{
    Sound stems[4];
    Sound menu[4];

}tGameSounds;

/*RECEBE UM INTEIRO, REALIZA UMA DIVISAO FLUTUANTE POR 60 E RETORNA SEU VALOR EM DECIMAL. GENERICO, DA PRA USAR PRA TUDO*/
float temporizador(int* cronometro)
{
    (*cronometro)++;
    return ((float)*cronometro)/90;
}

//acho que vou remover essa struct, mas enfim, coisa pra depois
typedef struct
{
    int grid_inicio[2];
    int grid_final[2];
}tCordenada;


void initializeWindow(){
    InitWindow(LARGURA, ALTURA, "PACMAN+"); 
    SetTargetFPS(90);
}

