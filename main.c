#include "header.h"

//MAIN
int main(void){
int move_x = 0, move_y = 0, move_alvo_x = 0, move_alvo_y = 0, grid_i, grid_j;
int score = 0;
char **grid_mapa;
char nome_mapa[50];
bool movimento_inicial = true, intencao_vertical = false, intencao_horizontal = false, virou = false, centro_grid = false, reverteu = false;
bool dentro_mapa = true, teleporte = false;
Rectangle *colisao_fantasma;
//Cores custom
Color CYAN = {0, 255, 255, 255}; 

//inicializacao jogador
tJogador pacman = {{}, 2, 3, false};

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

//Inicializações
InitWindow(LARGURA, ALTURA, "PACMAN+"); 
SetTargetFPS(60);

//inicia a matriz
int totalPellets = initMatrix("maps/mapa1.txt", grid_mapa);

//pos inicial do player
centralizaPlayer(&pacman, grid_mapa);

//Laço principal do jogo
while (!WindowShouldClose())
{
dentro_mapa = pacman.pos.x >= 0 && pacman.pos.x <= TAM_GRID*(TAM_J-1) && pacman.pos.y >= 0 && pacman.pos.y <= TAM_GRID*(TAM_I-1);
virou = false;
centro_grid = ((int)pacman.pos.x % TAM_GRID) == 0 && ((int)pacman.pos.y % TAM_GRID) == 0;

//input do menu de pause
if(IsKeyPressed(KEY_TAB)){
    //logica do menu
    bool unPause = false;
    while(!unPause){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("PAUSE", 10, 10, 40, YELLOW);
        DrawText("Aperte V para voltar", 10, 45, 20, YELLOW);
        DrawText("N: Novo Jogo", 10, 100, 20, PINK);
        DrawText("C: Carregar", 10, 150, 20, ORANGE);
        DrawText("S: Salvar", 10, 200, 20, CYAN);
        DrawText("Q: Sair", 10, 250, 20, RED);
        EndDrawing();

        if(IsKeyPressed(KEY_V)){
            unPause = true;
        }
        if(IsKeyPressed(KEY_N)){
            unPause = true;
            //resetGameState();
        }

        if(IsKeyPressed(KEY_Q)){
            CloseWindow();
        }
    }
}

//pegar o input
if(IsKeyPressed(KEY_RIGHT))
{
    intencao_horizontal = true;
    intencao_vertical = false;
    move_alvo_x = pacman.spd;
    move_alvo_y = 0;
}
if(IsKeyPressed(KEY_LEFT))
{
    intencao_horizontal = true;
    intencao_vertical = false;
    move_alvo_x = -pacman.spd;
    move_alvo_y = 0;
}
if(IsKeyPressed(KEY_UP))
{
    intencao_horizontal = false;
    intencao_vertical = true;
    move_alvo_x = 0;
    move_alvo_y = -pacman.spd;
}
if(IsKeyPressed(KEY_DOWN))
{
    intencao_horizontal = false;
    intencao_vertical = true;
    move_alvo_x = 0;
    move_alvo_y = pacman.spd;
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
if(dentro_mapa == true)
{
    grid_i = (int)pacman.pos.y/TAM_GRID;
    grid_j = (int)pacman.pos.x/TAM_GRID;

    //tomar a decisao de virar(caso seja necessario)
    if(centro_grid == true && reverteu == false)
    {
        //há intencao de mudar de eixo(do vertical pro horizontal ne)
        if(intencao_horizontal == true)
        {
            if((grid_mapa[grid_i][grid_j+(move_alvo_x)/pacman.spd]) != '#')
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
            if((grid_mapa[grid_i+(move_alvo_y/pacman.spd)][grid_j]) != '#')
            {
                move_y = move_alvo_y;
                move_x = 0;
                intencao_vertical = false;
                virou = true;
            }
        }
        //caso de colisao caso ele esteja andando reto em algum eixo
        //tive q adicionar grid_i != 0 na condicao por conta do caso particular em que o jogador ta no grid 0 se movimentando para cima, causando segmentation fault([0 +(-1)][])
        if(virou == false && (grid_i != 0 && grid_i != TAM_I-1) && (grid_mapa[grid_i+(move_y/pacman.spd)][grid_j+(move_x/pacman.spd)]) == '#')
        {
            move_x = 0;
            move_y = 0;
        }    
    }
}
reverteu = false;
//atualizacao da pos    
pacman.pos.x+= move_x; 
pacman.pos.y+= move_y;

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
        break;
        //power pellet
        case 'o':
            //logica do power pellet(a fazer)
            pacman.power_pellet = true;
            score+=50;
            grid_mapa[grid_i][grid_j] = ' ';
            totalPellets--;
        break;
        //fantasma(acho que vou fazer um sistema de colisoes a parte pra ele, suspeito que nao vai ficar uma colisao discreta dessa forma)
        /*case 'F':
            //logica
        break;
        */
        
        //portal
        /*
        case 'T':
            //to atualizando a pos do pacman dps de entrar no portal usando a spd so pra continuar sendo um multiplo de 2 e n dar dor de cabeca
            

        break;
        */
        
        //CONDICAO DE VITORIA
        if(totalPellets == 0)
        {
            //logica de vitoria(a fazer)
        }
    }
    
}
//teleporte player
teleporte = (pacman.pos.x == -40 || pacman.pos.x == TAM_GRID*(TAM_J) || pacman.pos.y == -40 || pacman.pos.y == TAM_GRID*(TAM_I));
if(dentro_mapa == false)
{
    if(teleporte == true)
    {
        teleportaPlayer(&pacman, move_x, move_y);
    }
}

//desenhos    
BeginDrawing(); 
ClearBackground(BLACK);
drawMap(grid_mapa);
DrawRectangle(pacman.pos.x, pacman.pos.y, TAM_GRID, TAM_GRID, YELLOW); 
DrawText(TextFormat("Score: %d", score), 10, 810, 20, WHITE);
DrawText(TextFormat("posx: %.2f, posy: %.2f", pacman.pos.x, pacman.pos.y), 900, 810, 20, WHITE);
DrawText(TextFormat("pellets: %d", totalPellets), 150, 810, 20, WHITE);
EndDrawing(); 
}

CloseWindow();

//liberacao de memoria
for(int i = 0; i < TAM_I; i++)
    free(*(grid_mapa+i));
free(grid_mapa);

return 0;
}