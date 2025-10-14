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

//funcoes
void initMatrix(char filename[], char **mapa);
void drawMap(char **mapa);


//MAIN
int main(void){
int input_x, input_y, move_x = 0, move_y = 0, move_alvo_x = 0, move_alvo_y = 0;
int spd = 2;
int score = 0;
int vida_player = 3;
float raio = 20;
char **grid_mapa;
int grid_i, grid_j;
bool movimento_inicial = true, intencao_vertical = false, intencao_horizontal = false, virou = false;
bool centro_grid = false, reverteu = false;

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

//calculo do movimento
virou = false;
centro_grid = ((int)(pos_player.x-(TAM_GRID/2)) % TAM_GRID) == 0 && ((int)(pos_player.y-(TAM_GRID/2)) % TAM_GRID) == 0;
if(IsKeyPressed(KEY_RIGHT))
{
    intencao_horizontal = true;
    intencao_vertical = false;
    move_alvo_x = spd;
    move_alvo_y = 0;
}
if(IsKeyPressed(KEY_LEFT))
{
    intencao_horizontal = true;
    intencao_vertical = false;
    move_alvo_x = -spd;
    move_alvo_y = 0;
}
if(IsKeyPressed(KEY_UP))
{
    intencao_horizontal = false;
    intencao_vertical = true;
    move_alvo_x = 0;
    move_alvo_y = -spd;
}
if(IsKeyPressed(KEY_DOWN))
{
    intencao_horizontal = false;
    intencao_vertical = true;
    move_alvo_x = 0;
    move_alvo_y = spd;
}

//impedir o delay aparente(se apertar pra se mover no msm eixo, ele n espera centralizar no grid)
if(intencao_horizontal == true && move_alvo_x == -move_x)
{
    move_x = move_alvo_x;
    intencao_horizontal = false;
    reverteu = true;
}
if(intencao_vertical == true && move_alvo_y == -move_y)
{
    move_y = move_alvo_y;
    intencao_vertical = false;
    reverteu = true;
}

grid_i = (int)pos_player.y/TAM_GRID;
grid_j = (int)pos_player.x/TAM_GRID;

//tomar a decisao de virar
if(centro_grid == true && reverteu == false)
{
    if(intencao_horizontal == true)
    {
        if((grid_mapa[grid_i][grid_j+(move_alvo_x)/spd]) != '#')
        {
            move_x = move_alvo_x;
            move_y = 0;
            intencao_horizontal = false;
            virou = true;
        }
    }
    else if(intencao_vertical == true)
    {
        if((grid_mapa[grid_i+(move_alvo_y/spd)][grid_j]) != '#')
        {
            move_y = move_alvo_y;
            move_x = 0;
            intencao_vertical = false;
            virou = true;
        }
    }
    if(virou == false && (grid_mapa[grid_i+(move_y/spd)][grid_j+(move_x/spd)]) == '#')
    {
        move_x = 0;
        move_y = 0;
    }    
}

reverteu = false;

//colisao parede
    
pos_player.x+= move_x; 
pos_player.y+= move_y;
   

//colisoes gerais
switch(grid_mapa[grid_i][grid_j])
{
    //pellet
    case '.':
        score+=10;
        grid_mapa[grid_i][grid_j] = ' ';
        break;
    //power pellet
    case 'o':
        //logica do power pellet(a fazer)
        score+=50;
        grid_mapa[grid_i][grid_j] = ' ';
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
DrawCircle(pos_player.x, pos_player.y, raio, YELLOW);
drawMap(grid_mapa);
DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
DrawText(TextFormat("posx: %.2f, posy: %.2f", pos_player.x, pos_player.y), 300, 10, 20, WHITE);
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
