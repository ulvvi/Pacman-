
#include "../header.h"

#pragma once


void drawGame(char** grid_mapa, Texture2D tileset_parede, Rectangle spritesheet, tJogador pacman, GameState state_atual, int** matriz_auxiliar){
    //layer fundo/mapa   
    BeginDrawing(); 
    ClearBackground(BLACK);
    drawMap(grid_mapa);
    drawTexturaParede(matriz_auxiliar, tileset_parede, spritesheet);
    //layer entidades
    DrawRectangle(pacman.pos.x, pacman.pos.y, TAM_GRID, TAM_GRID, YELLOW);
    //layer main HUD
    drawHUD(pacman.score, pacman.remainingPellets);
    DrawText(TextFormat("posx: %.2f, posy: %.2f", pacman.pos.x, pacman.pos.y), 900, 810, 20, WHITE);
}


void updateLogic(tJogador* pacman, char** grid_mapa, int* grid_i, int* grid_j, GameState* state_atual, int *option){

    if(IsKeyPressed(KEY_TAB))
    {
        *option = 0;
        *state_atual = PAUSE;
    }

    //movimentacao
    movePlayer(grid_mapa, pacman, grid_i, grid_j);

    //colisoes pellets
    if(checaPlayerCentralizado(pacman))
    {   
        colisaoPellets(pacman, grid_mapa, &pacman->score, &pacman->remainingPellets, *grid_i, *grid_j);
    }

    if(pacman->power_pellet == true)
    {
        powerPellet(pacman, state_atual);
    }

    //teleporte player
    if(checaPlayerDentroMapa(pacman) == false)
    {
        bool teleporte = (pacman->pos.x == -40 || pacman->pos.x == TAM_GRID*(TAM_J) || pacman->pos.y == -40 || pacman->pos.y == TAM_GRID*(TAM_I));
        if(teleporte == true)
        {
            teleportaPlayer(pacman);
        }
    }
}

void cleanup(char** grid_mapa, int** matriz_auxiliar, Texture2D cut_in, Sound som_cut_in, Texture2D tileset_parede){  
     //DAR UNLOAD NOS ASSETS
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
    UnloadTexture(tileset_parede);
    CloseAudioDevice();

    //liberar memoria
    freeDiddy(grid_mapa);
    freeMatrizAux(matriz_auxiliar);
}


void gameLevel(int level){

    int grid_i, grid_j, cronometro = 0;


    //dps mudar pro primeiro state ser o menu
    GameState state_atual = GAMEPLAY;
    Rectangle *colisao_fantasma;
   
    
    /*
    ***********************************
            MAIN WINDOW
    ***********************************
    */
    initializeWindow();


    /*
    ***********************************
                MENU
    ***********************************
    */
    int option = 0;

    /*
    ***********************************
                AUDIO
    ***********************************
    */
    Music stems[3];

    initiateAudio(stems, level);
    playMusic(stems);
    
    Sound som_cut_in = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(som_cut_in, 0.5f);
    
    
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
    int** matriz_auxiliar;
    char **grid_mapa = allocateMap();
    matriz_auxiliar = inicializaMatrizAux();
    initMap("maps/mapa1.txt", grid_mapa);
    texturaMapa(grid_mapa, matriz_auxiliar);

    
    /*
    ***********************************
                PLAYER
    ***********************************
    */
    tJogador pacman = {{}, 2, 3, 0, 0, false, 0, 250};
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

        //desenhos
        drawGame(grid_mapa, tileset_parede, spritesheet, pacman, state_atual, matriz_auxiliar);

        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:            
                switchMusic(GAMEPLAY, stems);
                updateLogic(&pacman, grid_mapa, &grid_i, &grid_j, &state_atual, &option);
            break;

            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&option, &state_atual, grid_mapa);
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                cutIn(som_cut_in, cut_in);
                if(cronometro == 0)
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems);
                    
                if(temporizador(&cronometro) >= 1.5)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                }
            break;
        }
        EndDrawing();
    }

    cleanup(grid_mapa, matriz_auxiliar, cut_in, som_cut_in, tileset_parede);
    return;
}