#pragma once
#include "../header.h"

/**
 * @brief Desenha o jogo na tela
 * @param mapa Mapa do jogo
 * @param pacman Estrutura do jogador
 * @param state_atual Estado atual do jogo
 * @param fantasmas Array de inimigos
 * @param camera_principal Camera principal do jogo
 * @param pontuacao Efeito visual de pontuação
 * @param assets Assets do jogo
 */
void drawGame(tMapa mapa, tJogador* pacman, GameState state_atual, tInimigo *fantasmas, tCamera camera_principal, tVfx* pontuacao, tAssets assets){
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
        if(pontuacao->ativo == true)
        {
            mostraPontuacao(pontuacao, *pacman);
        }
    }
    //fim do q a camera afeta
    EndMode2D();
    //layer main HUD
    drawHUD(pacman->score, pacman->remainingPellets, pacman->current_fruit, mapa, assets, pacman->vida);
}

/**
 * @brief Atualiza a lógica do jogo
 * @param pacman Estrutura do jogador
 * @param mapa Mapa do jogo
 * @param state_atual Estado atual do jogo
 * @param menuData Dados do menu
 * @param fantasma Array de inimigos
 * @param camera_principal Camera principal do jogo
 * @param pontuacao Efeito visual de pontuação
 * @param gameSFX Sons do jogo
 */
void updateLogic(tJogador* pacman, tMapa* mapa, GameState* state_atual, tMenu* menuData, tInimigo* fantasma, tCamera* camera_principal, tVfx* pontuacao, Sound gameSFX[5]){
    //contabilizador de frames pro fantasma 
    mapa->frame_counter++;
    if(IsKeyPressed(KEY_TAB))
    {
        menuData->index = 0;
        menuData->subIndex = 0;
        *state_atual = PAUSE;
    }

    //debug
    if(IsKeyPressed(KEY_F1)){
        pacman->current_fruit = CHERRY;
    }
    if(IsKeyPressed(KEY_F2)){
        pacman->current_fruit = STRAWBERRY;
    }
    if(IsKeyPressed(KEY_F3)){
        pacman->current_fruit = GRAPE;
    }
    if(IsKeyPressed(KEY_F4)){
        pacman->current_fruit = BLUEBERRY;
    }
    if(IsKeyDown(KEY_Q)){
        pacman->remainingPellets--;
    }

    //spawn de frutas
    spawnFruit(mapa->grid_mapa, pacman);

    //movimentacao
    movePlayer(mapa->grid_mapa, pacman);

    //colisoes pellets
    if(checaPlayerCentralizado(pacman) && checaPlayerDentroMapa(pacman))
    {   
        colisaoPellets(pacman, mapa->grid_mapa, &pacman->score, &pacman->remainingPellets, state_atual, pontuacao);
    }

    //cronometro do power pellet
    if(pacman->power_pellet == true)
    {
        powerPellet(pacman, mapa);
    }

    //cronometro das frutas e ativacao de seus respectivos poderes
    switch(pacman->fruta_ativa)
    {
        case GRAPE:
            grape(pacman, mapa->grid_mapa);
        break;

        case STRAWBERRY:
            strawberry(pacman, mapa->grid_mapa);
        break;

        case BLUEBERRY:
            blueberry(pacman);
        break;

        case CHERRY:
            cherry(pacman);
        break;
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
        fantasma[i] = moveFantasma(fantasma[i], fantasma[0], *mapa, mapa->frame_counter, *pacman);
        if(fantasma[i].desenho == false)
        {
            //so roda se tiver algum fantasma morto
            reviveFantasma(fantasma, i);
        }
    }
    atualizaColisaoFantasma(fantasma, mapa->numero_fantasmas);
    ConcretizaColisao(pacman, fantasma, mapa->grid_mapa, checaColisaoFantasma(pacman->colisao_player, fantasma, mapa->numero_fantasmas), mapa->numero_fantasmas, 
    state_atual, camera_principal, pontuacao, gameSFX);

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

/**
 * @brief Inicializa o nível do jogo
 * @param level Nível atual
 * @param mapa Mapa do jogo
 * @param pacman Estrutura do jogador
 * @param fantasmas Ponteiro para o array de inimigos
 * @param menu Dados do menu
 * @param stems Músicas do jogo
 * @param gameSFX Sons do jogo
 * @param obj_cut_in Animação de cut-in
 * @param assets Assets do jogo
 * @param obj_vitoria Animação de vitória
 * @param obj_confete Animação de confete
 */
void initGameLevel(int* level, tMapa* mapa, tJogador* pacman, tInimigo** fantasmas, tMenu* menu, Music stems[3], Sound* gameSFX, tAnimacao* obj_cut_in,
tAssets assets, tAnimacao* obj_vitoria, tAnimacao* obj_confete, tAnimacao* obj_derrota) 
{

    // --- ÁUDIO ---
    initiateAudio(stems, gameSFX, menu, *level);
    playMusic(stems);
    
    // --- MAPA e PLAYER ---
    char filename[30];
    modificaFilename(filename, *level);
    inicializaMapa(mapa, filename, level, assets);
    mapa->numero_fantasmas = calculaFantasmas(mapa->grid_mapa);
    inicializaPlayer(pacman, mapa->pellets_totais, assets);
    centralizaPlayer(pacman, mapa->grid_mapa);

    //--- MENU ---//
    menu->index = 0;
    menu->subIndex = 0;
    
    // --- INIMIGOS (com alocação de memória) ---
    *fantasmas = malloc(sizeof(tInimigo) * (mapa->numero_fantasmas));
    inicializaFantasmas(*fantasmas, mapa->grid_mapa, assets);
    
    // --- ANIMAÇÕES/CUTSCENES ---
    *obj_cut_in = (tAnimacao){
        0, 24, 0.075, 0, assets.cut_in_animacao, 
        {0,0,LARGURA, 600}, {0, ALTURA/2 - 600/2}, 0, 0, 0, 1
    };
    *obj_vitoria = (tAnimacao){
        0, 12, 0.080, 0, assets.vitoria_cutscene, 
        {0,0,LARGURA, ALTURA}, {0,0}, 0, 0, 0, 1
    };
    *obj_confete = (tAnimacao){
        0, 8, 0.160, 0, assets.confete_animacao, 
        {0,0,LARGURA, ALTURA}, {0,0}, 0, 0, 0, 10
    };
    *obj_derrota =  (tAnimacao){
        0, 12, 0.080, 0, assets.derrota_cutscene, 
        {0,0,LARGURA, ALTURA}, {0,0}, 0, 0, 0, 1
    };

}

/**
 * @brief Limpa os recursos alocados do nível do jogo
 * @param mapa Mapa do jogo
 * @param menuData Dados do menu
 * @param stems Músicas do jogo
 * @param fantasmas Array de inimigos
 * @param gameSFX Sons do jogo
 */
void cleanup(tMapa* mapa, tMenu* menuData, Music stems[], tInimigo* fantasmas, Sound gameSFX[5]){  
    //unload nos assets
    stopAllMusic(stems);

    for(int i = 0; i < 3; i++){
        UnloadMusicStream(stems[i]);
    }
    for(int i = 0; i < 6; i++){
        UnloadSound(gameSFX[i]);
    }
    //liberar memoria
    freeMascaras(mapa->mapa_mascaras);
    freeMap(mapa->grid_mapa);
    freeMatrizAux(mapa->matriz_auxiliar);
    free(fantasmas);
    
}

/**
 * @brief Executa o nível do jogo
 * @param level Nível atual
 * @param assets Assets do jogo
 * @param whatToDo Indica se deve carregar um jogo salvo
 * @return Retorna 1 se o jogador venceu, 0 caso contrário
 */
int gameLevel(int* level, tAssets assets, int whatToDo){
    int cronometro = 0;
    int venceu = 0;
    int firstTimeTransicao = 1;
    
    GameState state_atual = TRANSICAO;

    tMenu menuData;

    Music stems[3];
    Sound gameSFX[6];

    tMapa mapa = {0};
    
    tJogador pacman = {0};


    tInimigo* fantasmas;
    
    tAnimacao obj_cut_in;
    tAnimacao obj_transicao = {0, 18, 0.100, 0, assets.transicao_animacao,{0,0,LARGURA, ALTURA}, {0,0}, 0, 0, 0, 1};
    tAnimacao obj_vitoria;
    tAnimacao obj_confete;
    tAnimacao obj_derrota;

    tVfx pontuacao = {3, 3, false, {0,0}, 0};

    initGameLevel(level, &mapa, &pacman, &fantasmas, &menuData, stems, gameSFX, &obj_cut_in, assets, &obj_vitoria, &obj_confete, &obj_derrota);
    

    tCamera camera_principal;
    inicializaCamera(&camera_principal, pacman);
    PlaySound(gameSFX[0]);

    if(whatToDo == 2){
        load(&pacman, &fantasmas, &mapa, assets);
    } 

    int dangerPellets = pacman.remainingPellets / 3;
    

    /************************************
                JOGO
    ************************************/

    //tentei ate refatorar mas ficaria mt ruim
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);
        
        //desenhos
        drawGame(mapa, &pacman, state_atual, fantasmas, camera_principal, &pontuacao, assets);
             
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
                updateLogic(&pacman, &mapa, &state_atual, &menuData, fantasmas, &camera_principal, &pontuacao, gameSFX);
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
                    PlaySound(gameSFX[1]);
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
                        state_atual = DERROTA_CUTSCENE;
                    }
                    centralizaPlayer(&pacman, mapa.grid_mapa);
                    centralizaFantasma(fantasmas, mapa.numero_fantasmas);
                    pacman.desenho = true;
                }
            break;

            case PAUSE:
                switchMusic(MENU, stems);
                int aux = menuLogic(&menuData, &state_atual, &mapa, &pacman, &fantasmas, assets);
                if(aux != 0)
                {
                    //limpezas para retornar ao menu ou para iniciar novo jogo
                    cleanup(&mapa, &menuData, stems, fantasmas, gameSFX);
                    return aux;
                }
                
            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                static bool primeira_vez = true;
                if(primeira_vez)
                {
                    PlaySound(gameSFX[5]);
                    SetMusicVolume(stems[1], 0.25f);
                    trocaCorEXT(&mapa, 8);  
                    primeira_vez = false;
                }
                DrawRectangle(0,0, LARGURA, ALTURA, Fade(BLACK, 0.5f));
                cutscene(&obj_cut_in, &state_atual, GAMEPLAY);
                //a func cutscene ja troca o state quando acabar, logo, so sera gameplay apos a ultima chamada da func cutscene
                if(state_atual == GAMEPLAY)
                {
                    resumeAllMusic(stems);
                    SetMusicVolume(stems[1], 1.5f);
                    //trocaCorEXT(&mapa, cor_atual);     
                    primeira_vez = true;        
                }
                
            break;

            case VITORIA_CUTSCENE:
                pauseAllMusic(stems);
                cutscene(&obj_vitoria, &state_atual, GAMEPLAY);
                if(state_atual == GAMEPLAY)
                {
                    victoryScreen(obj_vitoria, obj_confete);
                    venceu = 1;
                    return venceu;
                }
            break;   
            
            case DERROTA_CUTSCENE:  
                pauseAllMusic(stems);
                cutscene(&obj_derrota, &state_atual, GAMEPLAY);
                if(state_atual == GAMEPLAY)
                {
                    // ClearBackground(BLACK);
                    // EndDrawing();
                    //limpezas pra dar game over
                    cleanup(&mapa, &menuData, stems, fantasmas, gameSFX);
                    venceu = 0;
                    return venceu;
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
                PlaySound(gameSFX[2]);
                state_atual = VITORIA_CUTSCENE;
            }
        }
    }

    cleanup(&mapa, &menuData, stems, fantasmas, gameSFX);
    return venceu;
}
