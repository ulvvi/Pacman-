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


//struct de animacao dos "objetos"
typedef struct
{
    int frame_atual;
    int total_frames;
    float tempo_frame;
    float contador;
    Texture2D sprite;
    Rectangle spritesheet;
    Vector2 pos;
    float rotacao;

} tAnimacao;


//struct do player
typedef struct
{
    Vector2 pos;
    int spd;
    int vida;
    int move_x;
    int move_y;
    bool power_pellet;
    float tempo_power_pellet;
    int score;
    int remainingPellets;
    int dir;

    bool desenho;
    Rectangle colisao_player;
    Rectangle spritesheet;
    Texture2D sprite;
    tAnimacao comendo;
    tAnimacao cutscene_morte;

} tJogador;

//struct do fantasma
typedef struct
{
    Vector2 pos;
    float spd;
    bool vulneravel;
    int direcao;
    Rectangle colisao_fantasma;
    Vector2 pos_inicial;

    Texture2D sprite;
    Rectangle spritesheet;
} tInimigo;

//struct do mapa
typedef struct 
{
    char** grid_mapa;
    int **matriz_auxiliar;
    int *mapa_mascaras;
    Texture2D tileset_parede; 
    Rectangle spritesheet;
    int tamanho_spritesheet;
    int pellets_totais;
    int frame_counter;

}tMapa;


//state machine principal do jogo
typedef enum GameState
{
    MENU,
    GAMEPLAY,
    JACKPOT,
    PAUSE,
    CUT_IN,
    PRIMEIRO_MOVIMENTO,
    MORTE

}GameState;




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


void initializeWindow(){
    InitWindow(LARGURA, ALTURA, "PACMAN+"); 
    SetTargetFPS(90);
}

