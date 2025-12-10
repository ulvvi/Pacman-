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

//Cores
Color ROXO = {119, 59, 191, 255};
Color ROXO_ESCURO = {78,39,140,255};

//struct dos possiveis vfx
typedef enum {
    CHERRY,
    STRAWBERRY,
    GRAPE,
    BLUEBERRY
} FruitType;


typedef struct
{
    float tempo;
    float tempo_restante;
    bool ativo;
    Vector2 pos;
    int parametro_especial;
}tVfx;


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
    int coluna_atual;
    int linha_atual;
    float escala;

} tAnimacao;

enum menuState{
  MAIN,
  SAVE_MENU,
  LOAD_MENU
};

typedef struct {
    int index;
    int subIndex;
    int MenuState;
    Sound menuSFX[2];
} tMenu;



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
    int current_fruit;
    FruitType fruta_ativa;
    float curFruitTimer;

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
    int type;

    float tempo_morto;
    bool desenho;
    Texture2D sprite_normal;
    Texture2D sprite_fuga;
    Rectangle spritesheet;

    tAnimacao morte;
    tAnimacao morto;
} tInimigo;

//struct do mapa
typedef struct 
{
    char** grid_mapa;
    int **matriz_auxiliar;
    int numero_fantasmas;
    int *mapa_mascaras;
    Texture2D tileset_parede; 
    Rectangle spritesheet;
    int tamanho_spritesheet;
    int pellets_totais;
    int frame_counter;
    int cor_atual;

    Texture2D cherry; 
    Texture2D grape; 
    Texture2D blueberry; 
    Texture2D strawberry; 
    int* level;

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
    MORTE,
    TRANSICAO,
    VITORIA_CUTSCENE,
    DERROTA_CUTSCENE
}GameState;




typedef struct
{
    Sound stems[4];
    Sound menu[4];

}tGameSounds;

/**
 * @brief Temporizador que converte frames em segundos.
 * @param cronometro Ponteiro para o contador de frames.
 */
float temporizador(int* cronometro)
{
    (*cronometro)++;
    return ((float)*cronometro)/100;
}

/**
 * @brief Inicializa a janela do jogo com as dimensões e título especificados.
 */
void initializeWindow(){
    InitWindow(LARGURA, ALTURA, "PACMAN+"); 
    SetTargetFPS(100);
}

