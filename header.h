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







