#include "libs/header.h"

//MAIN
int main(void)
{
    int** matriz_auxiliar;
    int grid_i, grid_j, cronometro = 0;
    int score = 0;
    char **grid_mapa;
    char nome_mapa[50];
    //dps mudar pro primeiro state ser o menu
    GameState state_atual = GAMEPLAY;
    Rectangle *colisao_fantasma;
    //Cores custom
    Color CYAN = {0, 255, 255, 255}; 

    //inicializacao jogador
    tJogador pacman = {{}, 2, 3, 0, 0, false};

    //alocacao dinamica do tamanho do mapa
    grid_mapa = allocateMap();
    matriz_auxiliar = inicializaMatrizAux();
   

    //Inicializações
    InitWindow(LARGURA, ALTURA, "PACMAN+"); 
    SetTargetFPS(60);
    InitAudioDevice();

    /*
    ***********************************
                AUDIO
    ***********************************
    */
    
    Sound som_cut_in = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(som_cut_in, 0.5f);

    //array de musicas
    Music stems[2];
    stems[0] = LoadMusicStream("audio/ambiente/lvl1.mp3);
    stems[1] = LoadMusicStream("audio/ambiente/menu_theme.wav");
    stems[2] = LoadMusicStream("audio/ambiente/pellet.wav");
    playMusic(stems);
    
    
    /*
    ***********************************
                TEXTURAS
    ***********************************
    */
    Texture2D cut_in = LoadTexture("sprites/player/pacman_cut_in.png");
    Rectangle spritesheet = {0, 0, 40, 40};
    Texture2D tileset_parede = LoadTexture("sprites/ambiente/tileset_paredes.png");


        
    /*
    ***********************************
                MAPA
    ***********************************
    */
    int totalPellets = initMap("maps/mapa1.txt", grid_mapa);
    texturaMapa(grid_mapa, matriz_auxiliar);

    
    /*
    ***********************************
                PLAYER
    ***********************************
    */
    centralizaPlayer(&pacman, grid_mapa);

    /*
    ***********************************
                JOGO
    ***********************************
    */
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);

        switch(state_atual)
        {
            //ESTADO PRINCIPAL
            case GAMEPLAY:
                //input do menu de pause
                if(IsKeyPressed(KEY_TAB))
                {
                    state_atual = PAUSE;
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
                    powerPellet(&pacman, &state_atual);
                }else{
                    
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
            break;
        }

        //desenhos


        //layer fundo/mapa   
        BeginDrawing(); 
        ClearBackground(BLACK);
        drawMap(grid_mapa);
        drawTexturaParede(matriz_auxiliar, tileset_parede, spritesheet);
        //layer entidades
        DrawRectangle(pacman.pos.x, pacman.pos.y, TAM_GRID, TAM_GRID, YELLOW);
        //layer main HUD
        drawHUD(score, totalPellets);
        DrawText(TextFormat("posx: %.2f, posy: %.2f", pacman.pos.x, pacman.pos.y), 900, 810, 20, WHITE);

        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:
                switchMusic(NORMAL, stems);
                
                if(pacman.power_pellet == true)
                {
                    switchMusic(POWER, stems);
                }    
            break;

            case PAUSE:
                //logica do menu
                switchMusic(MENU, stems);

                DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.8f));
                DrawText("PAUSE", 10, 10, 40, YELLOW);
                DrawText("Aperte V para voltar", 10, 45, 20, YELLOW);
                DrawText("N: Novo Jogo", 10, 100, 20, PINK);
                DrawText("C: Carregar", 10, 150, 20, ORANGE);
                DrawText("S: Salvar", 10, 200, 20, CYAN);
                DrawText("Q: Sair", 10, 250, 20, RED);
                if(IsKeyPressed(KEY_V)){
                    state_atual = GAMEPLAY;
                }
                if(IsKeyPressed(KEY_N)){
                    //resetGameState();
                }

                if(IsKeyPressed(KEY_Q)){
                    CloseWindow();
                    freeDiddy(grid_mapa);
                    return 0;
                }
            break;

            case CUT_IN:
                cutIn(som_cut_in, cut_in);
                if(cronometro == 0)
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems)
                    
                if(temporizador(&cronometro) >= 1.5)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems)
                }
            break;
        }
        //fim dos desenhos
        EndDrawing(); 
    }
    //DAR UNLOAD NOS ASSETS
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
    UnloadTexture(tileset_parede);

    CloseWindow();
    freeDiddy(grid_mapa);
    freeMatrizAux(matriz_auxiliar);
    return 0;
}




