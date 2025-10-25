#include "libs/header.h"

//MAIN
int main(void){
int grid_i, grid_j;
int score = 0;
char **grid_mapa;
char nome_mapa[50];
Rectangle *colisao_fantasma;
//Cores custom
Color CYAN = {0, 255, 255, 255}; 

//inicializacao jogador
tJogador pacman = {{}, 2, 3, 0, 0, false};

//alocacao dinamica do tamanho do mapa
grid_mapa = allocateMap();

//Inicializações
InitWindow(LARGURA, ALTURA, "PACMAN+"); 
SetTargetFPS(60);
InitAudioDevice();


//inicia a matriz
int totalPellets = initMap("maps/mapa1.txt", grid_mapa);

//pos inicial do player
centralizaPlayer(&pacman, grid_mapa);

//Laço principal do jogo
while (!WindowShouldClose())
{
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
            freeDiddy(grid_mapa);
            return 0;
        }
    }
}

//movimentacao
movePlayer(grid_mapa, &pacman, &grid_i, &grid_j);

//colisoes pellets
if(checaPlayerCentralizado(&pacman))
{   
    colisaoPellets(&pacman, grid_mapa, &score, &totalPellets, grid_i, grid_j);
}

if(pacman.power_pellet == true)
{
    powerPellet(&pacman);
}

//teleporte player
if(checaPlayerDentroMapa(&pacman) == false)
{
    bool teleporte = (pacman.pos.x == -40 || pacman.pos.x == TAM_GRID*(TAM_J) || pacman.pos.y == -40 || pacman.pos.y == TAM_GRID*(TAM_I));
    if(teleporte == true)
    {
        teleportaPlayer(&pacman);
    }
}

//desenhos    
BeginDrawing(); 
ClearBackground(BLACK);

drawMap(grid_mapa);
DrawRectangle(pacman.pos.x, pacman.pos.y, TAM_GRID, TAM_GRID, YELLOW);
drawHUD(score, totalPellets);
DrawText(TextFormat("posx: %.2f, posy: %.2f", pacman.pos.x, pacman.pos.y), 900, 810, 20, WHITE);
EndDrawing(); 
}

CloseWindow();

freeDiddy(grid_mapa);

return 0;
}
