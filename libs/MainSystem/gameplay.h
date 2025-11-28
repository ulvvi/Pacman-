#pragma once
#include "../header.h"



void drawGame(tMapa mapa, tJogador* pacman, GameState state_atual, tInimigo *fantasmas, tCamera camera_principal){
    //layer fundo/mapa   
    BeginDrawing(); 
    //iniciar camera(tudo entre beginmode2d e endmode2d que sofrera efeito de zoom e de screenshake, oq tiver fora, como hud, n sofrera)
    BeginMode2D(camera_principal.camera);
    ClearBackground(BLACK);
    drawMap(mapa);
    drawTexturaParede(mapa);
    //layer entidades
    if(pacman->desenho == true)
    {
        //pacman
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
    for(int i = 0; i < mapa.numero_fantasmas; i++)
    {
        if(fantasmas[i].desenho == true)
        {
            fantasmas[i].morte.pos.x = fantasmas[i].pos.x;
            fantasmas[i].morte.pos.y = fantasmas[i].pos.y;
            fantasmas[i].morte.frame_atual = 0;
            fantasmas[i].morto.frame_atual = 0;
            //decidir se desenha os sprites de fuga ou normais
            if(pacman->power_pellet == true)
            {
                DrawTextureRec(fantasmas[i].sprite_fuga, fantasmas[i].spritesheet, fantasmas[i].pos, WHITE);
            }
            else
            {
                DrawTextureRec(fantasmas[i].sprite_normal, fantasmas[i].spritesheet, fantasmas[i].pos, WHITE);
            }
            
        }
        else
        {
            //se chegou no fim da animacao de morte(uma alternativa seria criar um state pra rodar isso, mas ne, bemm desnecessario)
            if(fantasmas[i].morte.frame_atual == fantasmas[i].morte.total_frames-1)
            {
                //animacao de descanso
                animaObjeto(&fantasmas[i].morto);
            }
            else
            {
                //animacao de morte
                animaObjeto(&fantasmas[i].morte);
            }
        }
    }
    //fim do q a camera afeta
    EndMode2D();
    //layer main HUD
    drawHUD(pacman->score, pacman->remainingPellets, pacman->current_fruit, mapa);
    DrawText(TextFormat("posx: %.2f, posy: %.2f, vida: %d, dir: %d", pacman->pos.x, pacman->pos.y, pacman->vida, pacman->dir), 900, 810, 20, WHITE);
}

//updata o jogo a cada frame
void updateLogic(tJogador* pacman, tMapa* mapa, GameState* state_atual, tMenu* menuData, tInimigo* fantasma, tCamera* camera_principal){
    //contabilizador de frames pro fantasma 
    mapa->frame_counter++;
    if(IsKeyPressed(KEY_TAB))
    {
        menuData->index = 0;
        menuData->subIndex = 0;
        *state_atual = PAUSE;
    }

    //spawn de frutas
    spawnFruit(mapa->grid_mapa, pacman);

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
        powerPellet(pacman, mapa);
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

    //iteracao dos fantasmas
    for(int i = 0; i < mapa->numero_fantasmas; i++)
    {
        fantasma[i] = moveFantasma(fantasma[i], *mapa, mapa->frame_counter, *pacman);
        if(fantasma[i].desenho == false)
        {
            //so roda se tiver algum fantasma morto
            reviveFantasma(fantasma, i);
        }
    }
    atualizaColisaoFantasma(fantasma, mapa->numero_fantasmas);
    ConcretizaColisao(pacman, fantasma, mapa->grid_mapa, checaColisaoFantasma(pacman->colisao_player, fantasma, mapa->numero_fantasmas), mapa->numero_fantasmas, 
    state_atual, camera_principal);

    //n consegui encaixar esse troca sprite dentro da func do alexandre, por ela n receber um pointer
    if(camera_principal->ativa == true) screenShake(camera_principal);
    trocaSpriteFantasma(fantasma, mapa->numero_fantasmas);
}

bool hasCollectedAllPellets(tJogador* pacman){
    if(pacman->remainingPellets <= 0 || IsKeyPressed(KEY_C)){
        return true;
    } else {
        return false;
    }
}



void initGameLevel(int* level, tMapa* mapa, tJogador* pacman, tInimigo** fantasmas, tMenu* menu, Music stems[3], Sound* som_cut_in, Sound* jingle, tAnimacao* obj_cut_in) 
{

    // --- ÁUDIO ---
    initiateAudio(stems, menu, *level);
    playMusic(stems);
    *som_cut_in = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(*som_cut_in, 0.5f);
    *jingle = LoadSound("audio/ambiente/jingle.wav");
    SetSoundVolume(*jingle, 2.0f);
    
    // --- MAPA e PLAYER ---
    char filename[30];
    modificaFilename(filename, *level);
    inicializaMapa(mapa, filename, level);
    mapa->numero_fantasmas = calculaFantasmas(mapa->grid_mapa);

    mapa->tileset_parede = LoadTexture("sprites/ambiente/tileset_paredes.png");
    inicializaPlayer(pacman, mapa->pellets_totais);
    centralizaPlayer(pacman, mapa->grid_mapa);

    //--- MENU ---//
    menu->index = 0;
    menu->subIndex = 0;
    
    // --- INIMIGOS (com alocação de memória) ---
    *fantasmas = malloc(sizeof(tInimigo) * (mapa->numero_fantasmas));
    inicializaFantasmas(*fantasmas, mapa->grid_mapa);
    
    // --- ANIMAÇÕES/CUTSCENES ---
    *obj_cut_in = (tAnimacao){
        0, 26, 0.075, 0, LoadTexture("sprites/player/pacman_cut_in-Sheet.png"), 
        {0,0,LARGURA, 600}, {0, ALTURA/2 - 600/2}, 0, 0, 0
    };
}


//limpezas no geral
void cleanup(tMapa* mapa, tMenu* menuData, Music stems[], Sound som_cut_in){  
    //unload nos assets
    UnloadSound(som_cut_in);
    UnloadTexture(mapa->tileset_parede);
    stopAllMusic(stems);

    //liberar memoria
    freeMascaras(mapa->mapa_mascaras);
    freeDiddy(mapa->grid_mapa);
    freeMatrizAux(mapa->matriz_auxiliar);
}



bool gameLevel(int* level){
    int cronometro = 0;
    bool venceu = false;
    
    GameState state_atual = TRANSICAO;

    tMenu menuData;

    Music stems[3];
    
    Sound som_cut_in;
    Sound jingle;

    tMapa mapa = {0};
    
    tJogador pacman = {0};


    tInimigo* fantasmas;
    
    tAnimacao obj_cut_in;
    tAnimacao obj_transicao = {0, 18, 0.100, 0, LoadTexture("sprites/ambiente/transicao2-Sheet.png"),{0,0,LARGURA, ALTURA}, {0,0}, 0, 0, 0};

    initGameLevel(level, &mapa, &pacman, &fantasmas, &menuData, stems, &som_cut_in, &jingle, &obj_cut_in);

    tCamera camera_principal;
    inicializaCamera(&camera_principal, pacman);
    int dangerPellets = pacman.remainingPellets / 2;

    Sound winJingle = LoadSound("audio/ambiente/win_jingle.wav");
    SetSoundVolume(winJingle, 1.5f);
    
    
    /************************************
                JOGO
    ************************************/

    //tentei ate refatorar mas ficaria mt retardado
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);
        
        //desenhos
        drawGame(mapa, &pacman, state_atual, fantasmas, camera_principal);
       
        
        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:          
                switchMusic(GAMEPLAY, stems);
                if(pacman.power_pellet == true){
                    switchMusic(JACKPOT, stems);
                }
                if(pacman.remainingPellets <= dangerPellets){
                    SetMusicPitch(stems[0], 1.10f);
                    SetMusicPitch(stems[1], 1.10f);
                    SetMusicPitch(stems[2], 1.10f);
                }
                updateLogic(&pacman, &mapa, &state_atual, &menuData, fantasmas, &camera_principal);
                if(IsKeyDown(KEY_A)) 
                {
                    pacman.vida--; 
                    state_atual = MORTE;
                }
                pacman.comendo.pos.x = pacman.pos.x;
                pacman.comendo.pos.y = pacman.pos.y;
            break;

            case PRIMEIRO_MOVIMENTO:
                
                pauseAllMusic(stems);
                if(cronometro == 0){
                    PlaySound(jingle);
                    ativaCamera(&camera_principal, 4.0, 0);
                }
                if(temporizador(&cronometro) >= 5.0)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                }
                if(camera_principal.ativa == true) zoomInOut(&camera_principal, 1);
            break;

            case MORTE:
                if(camera_principal.ativa == true) screenShake(&camera_principal);
                pauseAllMusic(stems);
                pacman.cutscene_morte.pos.x = pacman.pos.x;
                pacman.cutscene_morte.pos.y = pacman.pos.y;

                pacman.desenho = false;
                //a func cutscene ja troca o state quando acabar, logo, so sera gameplay apos a ultima chamada da func cutscene
                cutscene(&pacman.cutscene_morte, &state_atual, TRANSICAO);
                if(state_atual == TRANSICAO)
                {
                    if(pacman.vida == 0)
                    {
                        return venceu;
                    }
                    centralizaPlayer(&pacman, mapa.grid_mapa);
                    centralizaFantasma(fantasmas, mapa.numero_fantasmas);
                    pacman.desenho = true;
                }
            break;

            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&menuData, &state_atual, &mapa, &pacman, fantasmas);
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                static bool primeira_vez = true;
                if(primeira_vez)
                {
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
                    //trocaCorEXT(&mapa, cor_atual);     
                    primeira_vez = true;        
                }
                
            break;
            
            case TRANSICAO:
               pauseAllMusic(stems); 
               camera_principal.camera.zoom = 8;
               cutscene(&obj_transicao, &state_atual, PRIMEIRO_MOVIMENTO);
            break;
        }
        EndDrawing();

        //logica de vitoria
        if(state_atual == GAMEPLAY){
            if(hasCollectedAllPellets(&pacman) == true){
                PlaySound(winJingle);
                victoryScreen();
                venceu = true;
                break;
            }
        }
    }

    cleanup(&mapa, &menuData, stems, som_cut_in);
    return venceu;
}
