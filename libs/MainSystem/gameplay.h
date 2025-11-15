#pragma once
#include "../header.h"


void drawGame(tMapa mapa, tJogador* pacman, GameState state_atual,int numero_fantasmas, tInimigo *fantasmas){
    //layer fundo/mapa   
    BeginDrawing(); 
    ClearBackground(BLACK);
    drawMap(mapa.grid_mapa);
    drawTexturaParede(mapa);
    //layer entidades
    //if(pacman.desenho == true) 
    if(pacman->desenho == true)
    {
        if(pacman->move_x != 0 || pacman->move_y != 0)
        {
            animaObjeto(&pacman->comendo);
        }
        else
        {
            DrawTextureRec(pacman->sprite, pacman->spritesheet, pacman->pos, WHITE);
        }
    }
    //DrawRectangleLinesEx(pacman.colisao_player, 4.0,RED);
    //jaja refatoro isso, é so q por enqt ainda to debuggando 
        for(int i = 0; i < numero_fantasmas; i++)
        {
            DrawTextureRec(fantasmas[i].sprite, fantasmas[i].spritesheet, fantasmas[i].pos, WHITE);
            
        }
    //layer main HUD
    drawHUD(pacman->score, pacman->remainingPellets);
    DrawText(TextFormat("posx: %.2f, posy: %.2f, vida: %d, dir: %d", pacman->pos.x, pacman->pos.y, pacman->vida, pacman->dir), 900, 810, 20, WHITE);
}


void updateLogic(tJogador* pacman, tMapa* mapa, GameState* state_atual, int *option, tInimigo* fantasma, int numero_fantasma){
    //contabilizador de frames pro fantasma 
    mapa->frame_counter++;

    if(IsKeyPressed(KEY_TAB))
    {
        *option = 0;
        *state_atual = PAUSE;
    }

    //movimentacao
    movePlayer(mapa->grid_mapa, pacman);

    //colisoes pellets
    if(checaPlayerCentralizado(pacman) && checaPlayerDentroMapa(pacman))
    {   
        colisaoPellets(pacman, mapa->grid_mapa, &pacman->score, &pacman->remainingPellets, state_atual);
    }

    //cronometro do power pellet
    if(pacman->power_pellet == true)
    {
        powerPellet(pacman);
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
        fantasma[i] = moveFantasma(fantasma[i], mapa->grid_mapa, mapa->frame_counter);
    }
    atualizaColisaoFantasma(fantasma, numero_fantasma);
    ConcretizaColisao(pacman, fantasma, mapa->grid_mapa, checaColisaoFantasma(pacman->colisao_player, fantasma, numero_fantasma), numero_fantasma, state_atual);
    //n consegui encaixar esse troca sprite dentro da func do alexandre, por ela n receber um pointer
    trocaSpriteFantasma(fantasma, numero_fantasma);
}

void cleanup(tMapa* mapa, Sound som_cut_in){  
    //unload nos assets
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
    Sound jingle = LoadSound("audio/ambiente/jingle.wav");
    SetSoundVolume(jingle, 0.5f);
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

    mapa.tileset_parede = LoadTexture("sprites/ambiente/tileset_paredes.png");
    tAnimacao obj_cut_in = {0, 26, 0.075, 0, LoadTexture("sprites/player/pacman_cut_in-Sheet.png"), {0,0,LARGURA, 600},  {0, ALTURA/2 - 600/2}, 0};


    /************************************
                JOGO
    ************************************/

    //tentei ate refatorar mas ficaria mt retardado
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);
        //desenhos
        drawGame(mapa, &pacman, state_atual,numero_fantasmas,fantasmas);
        
 
        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:          
                switchMusic(GAMEPLAY, stems);
                if(pacman.power_pellet == true){
                    switchMusic(JACKPOT, stems);
                }
                updateLogic(&pacman, &mapa, &state_atual, &option, fantasmas, numero_fantasmas);
                pacman.comendo.pos.x = pacman.pos.x;
                pacman.comendo.pos.y = pacman.pos.y;


                //depois tem que trocar essa porr
            break;

            case PRIMEIRO_MOVIMENTO:
                pauseAllMusic(stems);
                if(cronometro == 0){
                    PlaySound(jingle);
                }
                if(temporizador(&cronometro) >= 4.5)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                }
                //bool input = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN);
                /*if(input == true)
                {
                    updateLogic(&pacman, mapa.grid_mapa, &state_atual, &option, fantasmas, numero_fantasmas);
                    state_atual = GAMEPLAY;
                }*/
            break;

            case MORTE:
                pacman.cutscene_morte.pos.x = pacman.pos.x;
                pacman.cutscene_morte.pos.y = pacman.pos.y;
                pacman.desenho = false;
                //a func cutscene ja troca o state quando acabar, logo, so sera gameplay apos a ultima chamada da func cutscene
                cutscene(&pacman.cutscene_morte, &state_atual, PRIMEIRO_MOVIMENTO);
                if(state_atual == PRIMEIRO_MOVIMENTO)
                {
                    centralizaPlayer(&pacman, mapa.grid_mapa);
                    centralizaFantasma(fantasmas, numero_fantasmas);
                    pacman.desenho = true;
                }
            break;

            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&option, &state_atual, &mapa, &pacman, fantasmas, menuClick);
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                static bool primeira_vez = true;
                static int cor_atual;
                if(primeira_vez)
                {
                    cor_atual = mapa.spritesheet.y/40;
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems);
                    trocaCorEXT(&mapa, 8);  
                    primeira_vez = false;
                }
                DrawRectangle(0,0, LARGURA, ALTURA, Fade(BLACK, 0.5f));
                cutscene(&obj_cut_in, &state_atual, GAMEPLAY);
                //a func cutscene ja troca o state quando acabar, logo, so sera gameplay apos a ultima chamada da func cutscene
                if(state_atual == GAMEPLAY)
                {
                    resumeAllMusic(stems);
                    trocaCorEXT(&mapa, cor_atual);     
                    primeira_vez = true;        
                }
                
            break;
        }
        EndDrawing();
    }

    cleanup(&mapa, som_cut_in);
    return;
}