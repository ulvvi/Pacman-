#include "raylib.h"
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
int matriz[TAM_I][TAM_J];


//Inicializações
InitWindow(LARGURA, ALTURA, "Movimento"); 
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
    case '.':
        score++;
        matriz[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    case 'o':
        //logica do power pellet(a fazer)
        matriz[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    case 'F':
        vida_player--;
        //logica a mais de tomar dano
        break;
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
return 0;
}