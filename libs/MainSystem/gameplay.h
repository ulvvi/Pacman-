
#include "../header.h"

#pragma once


void drawGame(int** matriz_auxiliar, char** grid_mapa, Texture2D tileset_parede, Rectangle spritesheet, tJogador pacman, GameState state_atual, int* mapa_mascaras){
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


void updateLogic(tJogador* pacman, char** grid_mapa, GameState* state_atual, int *option, tInimigo* fantasma, Rectangle* colisao_fantasma, int numero_fantasma){
    static int frame_counter = 0;
    frame_counter++;
    if(IsKeyPressed(KEY_TAB))
    {
        *option = 0;
        *state_atual = PAUSE;
    }

    //movimentacao
    movePlayer(grid_mapa, pacman);

    //colisoes pellets
    if(checaPlayerCentralizado(pacman) && checaPlayerDentroMapa(pacman))
    {   
        colisaoPellets(pacman, grid_mapa, &pacman->score, &pacman->remainingPellets);
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

    //fantasmas
    for(int i = 0; i < numero_fantasma; i++)
    {
        fantasma[i] = moveFantasma(fantasma[i], grid_mapa, frame_counter);
    }
    atualizaColisaoFantasma(fantasma, colisao_fantasma, numero_fantasma);

    //frutas
    fruitSpawn(grid_mapa);


}

void cleanup(char** grid_mapa, int** matriz_auxiliar, int* mapa_mascaras, Texture2D cut_in, Sound som_cut_in, Texture2D tileset_parede){  
    //DAR UNLOAD NOS ASSETS
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
    UnloadTexture(tileset_parede);
    CloseAudioDevice();

    //liberar memoria
    freeMascaras(mapa_mascaras);
    freeDiddy(grid_mapa);
    freeMatrizAux(matriz_auxiliar);
}


void gameLevel(int level){

    int cronometro = 0;
    

    //dps mudar pro primeiro state ser o menu
    GameState state_atual = GAMEPLAY;
    Rectangle colisao_player = {0,0,40,40};
   
    
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
    Sound menuClick = LoadSound("audio/menuSFX/menu1.wav");
    SetSoundVolume(menuClick, 1.2f);
    
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
    int* mapa_mascaras;
    char **grid_mapa = allocateMap();
    
    mapa_mascaras = malloc(sizeof(int)*256);
    inicializaMapeamento(mapa_mascaras, 256);
    matriz_auxiliar = inicializaMatrizAux();
    initMap("maps/mapa1.txt", grid_mapa);
    texturizaMapa(matriz_auxiliar, mapa_mascaras, grid_mapa);
    

    
    /*
    ***********************************
                PLAYER
    ***********************************
    */
    tJogador pacman = {{}, 2, 3, 0, 0, false, 0, 250, 1};
    centralizaPlayer(&pacman, grid_mapa);
    

    /*
    ***********************************
                INIMIGO
    ***********************************
    */  
    int numero_fantasmas = calculaFantasmas(grid_mapa);
    Rectangle *colisao_fantasma = criaColisaoFantasma(numero_fantasmas);
    tInimigo* fantasmas = malloc(sizeof(tInimigo)*numero_fantasmas);
    inicializaFantasmas(fantasmas, grid_mapa);




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
        drawGame(matriz_auxiliar, grid_mapa, tileset_parede, spritesheet, pacman, state_atual, mapa_mascaras);
        for(int i = 0; i < numero_fantasmas; i++)
        {
            DrawRectangle(fantasmas[i].pos.x, fantasmas[i].pos.y, TAM_GRID, TAM_GRID, WHITE);
            DrawRectangleLinesEx(colisao_fantasma[i], 1.0,RED);
        }
        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:            
                switchMusic(GAMEPLAY, stems);
                updateLogic(&pacman, grid_mapa, &state_atual, &option, fantasmas,colisao_fantasma,numero_fantasmas);
                if(pacman.power_pellet == true){
                    switchMusic(JACKPOT, stems);
                }
            break;

            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&option, &state_atual, grid_mapa, menuClick);
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                cutIn(som_cut_in, cut_in);
                if(cronometro == 0)
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems);
                    
                if(temporizador(&cronometro) >= 1.0)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                }
            break;
        }
        EndDrawing();
    }

    cleanup(grid_mapa, matriz_auxiliar, mapa_mascaras, cut_in, som_cut_in, tileset_parede);
    return;
}