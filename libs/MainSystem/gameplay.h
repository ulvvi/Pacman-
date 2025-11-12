
#include "../header.h"

#pragma once


void drawGame(tMapa mapa, tJogador pacman, GameState state_atual){
    //layer fundo/mapa   
    BeginDrawing(); 
    ClearBackground(DARKGRAY);
    drawMap(mapa.grid_mapa);
    drawTexturaParede(mapa);
    //layer entidades
    
    DrawTextureRec(pacman.sprite, pacman.spritesheet, pacman.pos, WHITE);
    DrawRectangleLinesEx(pacman.colisao_player, 4.0,RED);
    //layer main HUD
    drawHUD(pacman.score, pacman.remainingPellets);
    DrawText(TextFormat("posx: %.2f, posy: %.2f, vida: %d, dir: %d", pacman.pos.x, pacman.pos.y, pacman.vida, pacman.dir), 900, 810, 20, WHITE);
}


void updateLogic(tJogador* pacman, char** grid_mapa, GameState* state_atual, int *option, tInimigo* fantasma, int numero_fantasma){
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
    atualizaColisaoFantasma(fantasma, numero_fantasma);
    ConcretizaColisao(pacman, fantasma, grid_mapa, checaColisaoFantasma(pacman->colisao_player, fantasma, numero_fantasma), numero_fantasma, state_atual);
}

void cleanup(tMapa* mapa, Texture2D cut_in, Sound som_cut_in){  
    //DAR UNLOAD NOS ASSETS
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
    UnloadTexture(mapa->tileset_parede);
    CloseAudioDevice();

    //liberar memoria
    freeMascaras(mapa->mapa_mascaras);
    freeDiddy(mapa->grid_mapa);
    freeMatrizAux(mapa->matriz_auxiliar);
}


void gameLevel(int level){

    int cronometro = 0;
    //dps mudar pro primeiro state ser o menu
    GameState state_atual = PRIMEIRO_MOVIMENTO;

    /************************************
                MENU
    ************************************/
    int option = 0;

    /***********************************
                AUDIO
    ************************************/
    Music stems[3];

    initiateAudio(stems, level);
    playMusic(stems);
    
    Sound som_cut_in = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(som_cut_in, 0.5f);
    Sound menuClick = LoadSound("audio/menuSFX/menu1.wav");
    SetSoundVolume(menuClick, 1.2f);
    

    /************************************
                MAPA
    ************************************/
    tMapa mapa;
    inicializaMapa(&mapa);
    

    /************************************
                PLAYER
    ************************************/
    tJogador pacman;
    inicializaPlayer(&pacman, mapa.pellets_totais);
    centralizaPlayer(&pacman, mapa.grid_mapa);
    
    /************************************
                INIMIGO
    ************************************/  
    int numero_fantasmas = calculaFantasmas(mapa.grid_mapa);
    tInimigo* fantasmas = malloc(sizeof(tInimigo)*numero_fantasmas);
    inicializaFantasmas(fantasmas, mapa.grid_mapa);
    


        
    /************************************
                TEXTURAS
    ************************************/
    Texture2D cut_in = LoadTexture("sprites/player/pacman_cut_in.png");
    mapa.tileset_parede = LoadTexture("sprites/ambiente/tileset_paredes.png");
    //jaja refatoro essas coisas da textura
    pacman.sprite = LoadTexture("sprites/player/pacman_spritesheet.png");
    pacman.spritesheet.height = 40;
    pacman.spritesheet.width = 40;
    pacman.spritesheet.x = 0;
    pacman.spritesheet.y = 0;
    for(int i = 0; i < numero_fantasmas; i++)
    {
        fantasmas[i].sprite = LoadTexture("sprites/inimigo/fantasma_spritesheet.png");
        fantasmas[i].spritesheet.height = 40;
        fantasmas[i].spritesheet.width = 40;
        fantasmas[i].spritesheet.x = 0;
        fantasmas[i].spritesheet.y = 0;
    }


    /************************************
                JOGO
    ************************************/

    //tentei ate refatorar mas ficaria mt retardado
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);
        //desenhos
        drawGame(mapa, pacman, state_atual);
        
        //jaja refatoro isso, é so q por enqt ainda to debuggando 
        for(int i = 0; i < numero_fantasmas; i++)
        {
            DrawRectangle(fantasmas[i].pos.x, fantasmas[i].pos.y, TAM_GRID, TAM_GRID, WHITE);
            //DrawRectangleLinesEx(fantasmas[i].colisao_fantasma, 1.0,RED);
            
        }
 
        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:            
                switchMusic(GAMEPLAY, stems);
                updateLogic(&pacman, mapa.grid_mapa, &state_atual, &option, fantasmas, numero_fantasmas);
                //depois tem que trocar essa porra
                if(pacman.power_pellet == true){
                    switchMusic(JACKPOT, stems);
                }
            break;

            case PRIMEIRO_MOVIMENTO:
                bool input = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN);
                if(input == true)
                {
                    updateLogic(&pacman, mapa.grid_mapa, &state_atual, &option, fantasmas, numero_fantasmas);
                    state_atual = GAMEPLAY;
                }
            break;
            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&option, &state_atual, mapa.grid_mapa, menuClick);
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                static int cor_atual;
                cutIn(som_cut_in, cut_in);
                if(cronometro == 0)
                {
                    cor_atual = mapa.spritesheet.y/40;
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems);
                    trocaCorEXT(&mapa, 8);
                }    
                if(temporizador(&cronometro) >= 1.0)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                    trocaCorEXT(&mapa, cor_atual);
                    
                }
                
            break;
        }
        EndDrawing();
    }

    cleanup(&mapa, cut_in, som_cut_in);
    return;
}