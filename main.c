#include "header.h"


//MAIN
int main(void){
int move_x = 0, move_y = 0, move_alvo_x = 0, move_alvo_y = 0, spd = 2, grid_i, grid_j;
int score = 0;
int vida_player = 3;
float raio = 20;
char **grid_mapa;
char nome_mapa[50];
bool movimento_inicial = true, intencao_vertical = false, intencao_horizontal = false, virou = false, centro_grid = false, reverteu = false;

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
int totalPellets = initMatrix("maps/mapa1.txt", grid_mapa);

//pos inicial do player
Vector2 pos_player;
centralizaPlayer(&pos_player, grid_mapa);

//Inicializações
InitWindow(LARGURA, ALTURA, "PACMAN+"); 
SetTargetFPS(60);

//Laço principal do jogo
while (!WindowShouldClose())
{

virou = false;
centro_grid = ((int)pos_player.x % TAM_GRID) == 0 && ((int)pos_player.y % TAM_GRID) == 0;
//pegar o input
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

//impedir o delay aparente(se apertar pra se mover no msm eixo, ele n espera centralizar no grid, os dois ifs sao pra isso)
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
//grid atual do player
grid_i = (int)pos_player.y/TAM_GRID;
grid_j = (int)pos_player.x/TAM_GRID;

//tomar a decisao de virar(caso seja necessario)
if(centro_grid == true && reverteu == false)
{
    //há intencao de mudar de eixo(do vertical pro horizontal ne)
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
    //há intencao de mudar de eixo
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
    //caso de colisao caso ele esteja andando reto em algum eixo
    if(virou == false && (grid_mapa[grid_i+(move_y/spd)][grid_j+(move_x/spd)]) == '#')
    {
        move_x = 0;
        move_y = 0;
    }    
}

reverteu = false;
//atualizacao da pos    
pos_player.x+= move_x; 
pos_player.y+= move_y;
   

//colisoes gerais
if(centro_grid == true)
{   
    switch(grid_mapa[grid_i][grid_j])
    {
        //pellet
        case '.':
            score+=10;
            grid_mapa[grid_i][grid_j] = ' ';
            totalPellets--;
            if(totalPellets == 0)
            {
                //logica de vitoria(a fazer)
            }
            break;
        //power pellet
        case 'o':
            //logica do power pellet(a fazer)
            score+=50;
            grid_mapa[grid_i][grid_j] = ' ';
            totalPellets--;
            if(totalPellets == 0)
            {
                //logica de vitoria(a fazer)
            }
            break;
        //fantasma(acho que vou fazer um sistema de colisoes a parte pra ele, suspeito que nao vai ficar uma colisao discreta dessa forma)
        /*case 'F':
            //logica
            vida_player--;
            break;
        */
        //portal
        case 'T':
            /*if()
            {
                //
            }    
            else if(move_y == input_y)
            {
                //
            }
            break;
            */
            
    }
    
}
 
//desenhos    
BeginDrawing(); 
ClearBackground(BLACK);
drawMap(grid_mapa);
DrawRectangle(pos_player.x, pos_player.y, TAM_GRID, TAM_GRID, YELLOW); 
DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
DrawText(TextFormat("posx: %.2f, posy: %.2f", pos_player.x, pos_player.y), 300, 10, 20, WHITE);
DrawText(TextFormat("pellets: %d", totalPellets), 900, 10, 20, WHITE);
EndDrawing(); 
}

CloseWindow();

//liberacao de memoria
for(int i = 0; i < TAM_I; i++)
    free(*(grid_mapa+i));
free(grid_mapa);

return 0;
}