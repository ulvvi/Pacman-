#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#define LARGURA 1600
#define ALTURA 840
#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40

int main(void){
Vector2 pos_player = {400,400};
int input_x, input_y, move_x = 0, move_y = 0, move_alvo_x = 0, move_alvo_y = 0;
float spd = 4/3;
int score = 0;
int vida_player = 3;
char **matriz;

//alocacao dinamica do tamanho do mapa
matriz = (char**)malloc(sizeof(char*)*TAM_I);
if(matriz == NULL)
    return -1;

for(int i = 0; i < TAM_I; i++)
{
    *(matriz+i) = (char*)malloc(sizeof(char)*TAM_J);
    if(*(matriz+i) == NULL)
        return -1;
}

//Inicializações
InitWindow(LARGURA, ALTURA, "PACMAN-"); 
SetTargetFPS(60);


//Laço principal do jogo
while (!WindowShouldClose())
{

//movimentacao
input_x = (IsKeyPressed(KEY_RIGHT) - IsKeyPressed(KEY_LEFT))*spd;
if(input_x != 0)
{
    move_alvo_x = input_x;
    move_alvo_y = 0;
}
else if((input_y = (IsKeyPressed(KEY_DOWN) - IsKeyPressed(KEY_UP))*spd) != 0)
{
    move_alvo_y = input_y;
    move_alvo_x = 0;
}
if(move_alvo_x != move_x)
    move_x = move_alvo_x;    
if(move_alvo_y != move_y)
    move_y = move_alvo_y;

//atualizacao da pos
if ((matriz[(int)(pos_player.y+move_y)/TAM_GRID][(int)(pos_player.x+move_x)/TAM_GRID]) != '#')
    pos_player.x+=move_x;
if ((matriz[(int)(pos_player.y+move_y)/TAM_GRID][(int)(pos_player.x+move_x)/TAM_GRID]) != '#')
    pos_player.y+=move_y;

//colisoes gerais
switch(matriz[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)])
{
    //pellet
    case '.':
        score+=10;
        matriz[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    //power pellet
    case 'o':
        //logica do power pellet(a fazer)
        score+=50;
        matriz[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    //fantasma
    case 'F':
        //dps ver o bagulho de flags e tudo mais, caso o pacman possa ou nao comer os fantasmas
        vida_player--;
        break;
    //portal
    case 'T':
        //logica do portal
        break;
        
}
 
    
BeginDrawing(); 
ClearBackground(RAYWHITE); 
DrawRectangle(pos_player.x, pos_player.y, 20, 20, RED);
DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
EndDrawing(); 
}

CloseWindow();

//liberacao de memoria
for(int i = 0; i < TAM_I; i++)
    free(*(matriz+i));
free(matriz);

return 0;
}
