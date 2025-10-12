#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
//macros
#define LARGURA 1600
#define ALTURA 840
#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40
//funcoes
void initMatrix(char filename[], char **mapa);
void drawMap(char **mapa);



//MAIN
int main(void){

int input_x, input_y, move_x = 0, move_y = 0, move_alvo_x = 0, move_alvo_y = 0;
float spd = 4/3;
int score = 0;
int vida_player = 3;
char **grid_mapa;

//alocacao dinamica do tamanho do mapa
grid_mapa = (char**)malloc(sizeof(char*)*TAM_I);
if(grid_mapa == NULL)
    return -1;

for(int i = 0; i < TAM_I; i++)
{
    *(grid_mapa+i) = (char*)malloc(sizeof(char)*TAM_J);
    if(*(grid_mapa+i) == NULL)
        return -1;
}

//inicia a matriz
initMatrix("level1.txt", grid_mapa);

//pos inicial do player
Vector2 pos_player;

for(int i = 0; i < TAM_I; i++)
{
    for(int j = 0; j < TAM_J; j++)
    {
        if(grid_mapa[i][j] == 'P')
        {
            pos_player.x = j * TAM_GRID + TAM_GRID / 2;
            pos_player.y = i * TAM_GRID + TAM_GRID / 2;
            break;
        }
    }
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
if ((grid_mapa[(int)(pos_player.y+move_y)/TAM_GRID][(int)(pos_player.x+move_x)/TAM_GRID]) != '#')
    pos_player.x+=move_x;
if ((grid_mapa[(int)(pos_player.y+move_y)/TAM_GRID][(int)(pos_player.x+move_x)/TAM_GRID]) != '#')
    pos_player.y+=move_y;

//colisoes gerais
switch(grid_mapa[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)])
{
    //pellet
    case '.':
        score+=10;
        grid_mapa[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    //power pellet
    case 'o':
        //logica do power pellet(a fazer)
        score+=50;
        grid_mapa[(int)(pos_player.y/TAM_GRID)][(int)(pos_player.x/TAM_GRID)] = ' ';
        break;
    //fantasma(acho que vou fazer um sistema de colisoes a parte pra ele, suspeito que nao vai ficar uma colisao discreta dessa forma)
    /*case 'F':
        //logica
        vida_player--;
        break;
    */
    //portal
    case 'T':
        if(move_x == input_x)
        {
            //
        }    
        else if(move_y == input_y)
        {
            //
        }
        break;
        
}
 
//desenhos    
BeginDrawing(); 
ClearBackground(BLACK); 
DrawCircle(pos_player.x, pos_player.y, 20, YELLOW);
drawMap(grid_mapa);
DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
EndDrawing(); 
}

CloseWindow();

//liberacao de memoria
for(int i = 0; i < TAM_I; i++)
    free(*(grid_mapa+i));
free(grid_mapa);

return 0;
}

//inicializa a matriz com o nome do arq
void initMatrix(char filename[], char **mapa) {
    ///abrir
    FILE *map;
    map = fopen(filename, "r");
    if(map == NULL)
    {
        puts("erro de abertura");
        exit(1);
    }
    //coloca as informação do arquivo para uma matriz que será o mapa;
    char cur;
    int a = 0;
    int b = 0;
    while((cur = fgetc(map)) != EOF) 
    {
        if(cur == '\n') 
        {
            a++;
            b = 0;
        } 
        else 
        {
            mapa[a][b] = cur;
            b++;
        }      
    }
    ///fechar
    fclose(map);
}

//desenha o mapa com a matriz
void drawMap(char** mapa) 
{
    for(int i = 0; i < TAM_I; i++) 
    {
        for(int j = 0; j < TAM_J; j++)
        {
            float x = j * TAM_GRID;
            float y = i * TAM_GRID;
            float center_x = j * TAM_GRID + TAM_GRID / 2;
            float center_y = i * TAM_GRID + TAM_GRID / 2;

            switch(mapa[i][j])
            {
                //desenhar parede
                case '#':
                    DrawRectangle(x, y, 40, 40, BLUE);

                    break;
                //desenhar pellet
                case '.':
                    DrawCircle(center_x, center_y, 5, ORANGE);

                    break;
                //desenhar super pellet
                case 'o':
                    DrawCircle(center_x, center_y, 10, ORANGE);

                    break;
                case 'T':
                    DrawRectangle(x, y, 40, 40, PURPLE);
                    break;
                //desenhar(ou n kkkkkkkkkkkkk) os vazios
                case ' ':
                    break;
                //bizzarrices
                default:
                    break;
            }
        }
    }
}