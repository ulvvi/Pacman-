//system.h
#pragma once
#include "../header.h"

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
    int score;
    int remainingPellets;
    int dir;

} tJogador;
typedef struct
{
    //vai ter q criar um array dessa struct se pa na main, dinamicamente
    //vector2 tem os campos x e y
    Vector2 pos;
    int spd;
    bool vulneravel;
} tInimigo;


//STATE MACHINE eba memorias de guerra
typedef enum GameState
{
    MENU,
    GAMEPLAY,
    JACKPOT,
    PAUSE,
    CUT_IN

}GameState;



/*RECEBE UM INTEIRO, REALIZA UMA DIVISAO FLUTUANTE POR 60 E RETORNA SEU VALOR EM DECIMAL. GENERICO, DA PRA USAR PRA TUDO*/
float temporizador(int* cronometro)
{
    (*cronometro)++;
    return ((float)*cronometro)/60;
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
