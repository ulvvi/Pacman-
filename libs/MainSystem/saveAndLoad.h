#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#include "../Character/player.h"

/**
 * @brief Cria um novo fantasma com os atributos iniciais
 * @param fantasma Ponteiro para o array de inimigos
 * @param assets Assets do jogo
 * @param mapa Mapa do jogo
 */
void criaNovoFantasma(tInimigo** fantasma, tAssets assets, tMapa mapa)
{
    for(int i = 0; i < mapa.numero_fantasmas; i++)
    {
        //n eh um array bidimensional ok, é so q to tendo q usar o endereco do endereco de fantasmas, ent so assim pra acessar(ate onde eu sei)
        //to tendo q usar endereco de endereco pq o realloc antes realocava a copia do endereco de memoria passada pelo argumento da funcao. agr eu to passando a copia do
        //endereco do endereco pra ai sim mexer no endereco de fato, q n é uma copia
        fantasma[0][i].desenho = true;
        fantasma[0][i].sprite_normal = assets.fantasma_normal_sprite;
        fantasma[0][i].sprite_fuga = assets.fantasma_fuga_sprite;
        fantasma[0][i].spritesheet.height = 40;
        fantasma[0][i].spritesheet.width = 40;
        fantasma[0][i].spritesheet.x = 0;
        fantasma[0][i].spritesheet.y = 40*(i%4);

        //animacao morte
        fantasma[0][i].morte.frame_atual = 0;
        fantasma[0][i].morte.total_frames = 14;
        fantasma[0][i].morte.tempo_frame = 0.110;
        fantasma[0][i].morte.sprite = assets.fantasma_morte_sprite;
        fantasma[0][i].morte.spritesheet.width = 40;
        fantasma[0][i].morte.spritesheet.height = 40;
        fantasma[0][i].morte.spritesheet.x = 0;
        fantasma[0][i].morte.spritesheet.y = fantasma[0][i].spritesheet.y;
        fantasma[0][i].morte.rotacao = 0;
        fantasma[0][i].morte.escala = 1;

        //animacao morto(descanso)
        fantasma[0][i].morto.frame_atual = 0;
        fantasma[0][i].morto.total_frames = 7;
        fantasma[0][i].morto.tempo_frame = 0.130;
        fantasma[0][i].morto.sprite = assets.fantasma_morto_sprite;
        fantasma[0][i].morto.spritesheet.width = 40;
        fantasma[0][i].morto.spritesheet.height = 40;
        fantasma[0][i].morto.spritesheet.x = 0;
        fantasma[0][i].morto.spritesheet.y = fantasma[0][i].spritesheet.y;
        fantasma[0][i].morto.rotacao = 0;
        fantasma[0][i].morto.pos.x = fantasma[0][i].pos_inicial.x;
        fantasma[0][i].morto.pos.y = fantasma[0][i].pos_inicial.y;
        fantasma[0][i].morto.escala = 1;
    }
}

/**
 * @brief Salva os dados do jogador em um arquivo binário
 * @param pacman Ponteiro para a estrutura do jogador
 * @param arq Ponteiro para o arquivo binário
 */
void save_player(tJogador* pacman, FILE* arq)
{
    fwrite(&pacman->pos.x, sizeof(float), 1, arq);
    fwrite(&pacman->pos.y, sizeof(float), 1, arq);
    fwrite(&pacman->dir, sizeof(int), 1, arq);
    fwrite(&pacman->move_x, sizeof(int), 1, arq);
    fwrite(&pacman->move_y,sizeof(int), 1, arq);
    fwrite(&pacman->spd, sizeof(int), 1, arq);
    fwrite(&pacman->power_pellet, sizeof(bool), 1, arq);
    fwrite(&pacman->score, sizeof(int), 1, arq);
    fwrite(&pacman->vida, sizeof(int), 1, arq); 
    fwrite(&pacman->remainingPellets, sizeof(int), 1, arq);
    fwrite(&pacman->desenho, sizeof(bool), 1, arq);
    fwrite(&pacman->tempo_power_pellet, sizeof(float), 1, arq);
    fwrite(&pacman->current_fruit, sizeof(int), 1, arq);
    fwrite(&pacman->comendo.rotacao, sizeof(int),1, arq);
    fwrite(&pacman->comendo.spritesheet.width, sizeof(int), 1, arq);
    fwrite(&pacman->fruta_ativa, sizeof(FruitType), 1, arq);
    fwrite(&pacman->curFruitTimer, sizeof(float), 1, arq);
    fwrite(&pacman->current_fruit, sizeof(int), 1, arq);
}

/**
 * @brief Salva os dados dos inimigos em um arquivo binário
 * @param fantasma Ponteiro para o array de inimigos
 * @param map Mapa do jogo
 * @param arq Ponteiro para o arquivo binário
 */
void save_fantasma(tInimigo* fantasma, tMapa map, FILE* arq)
{
    for(int i = 0; i < map.numero_fantasmas; i++)
    {
        fwrite(&fantasma[i].spd, sizeof(float), 1, arq);
        fwrite(&fantasma[i].vulneravel, sizeof(bool), 1, arq);;
        fwrite(&fantasma[i].direcao, sizeof(int), 1, arq);;
        fwrite(&fantasma[i].tempo_morto, sizeof(float), 1, arq);; 
        fwrite(&fantasma[i].pos, sizeof(Vector2), 1, arq);
        fwrite(&fantasma[i].pos_inicial, sizeof(Vector2), 1, arq);
        fwrite(&fantasma[i].type, sizeof(int), 1, arq);
        fwrite(&fantasma[i].desenho, sizeof(bool), 1, arq);
        fwrite(&fantasma[i].morte.contador, sizeof(float), 1, arq);
        fwrite(&fantasma[i].morto.contador, sizeof(float), 1, arq);
        fwrite(&fantasma[i].colisao_fantasma, sizeof(Rectangle),1,arq);

    }
}

/**
 * @brief Salva os dados do mapa em um arquivo binário
 * @param mapa Ponteiro para a estrutura do mapa
 * @param fp Ponteiro para o arquivo binário
 */
void save_map(tMapa* mapa, FILE* fp) {

    //fwrite(&mapa->tileset_parede, sizeof(Texture2D), 1, fp);
    fwrite(&mapa->spritesheet, sizeof(Rectangle), 1, fp);
    fwrite(&mapa->tamanho_spritesheet, sizeof(int), 1, fp);
    fwrite(&mapa->pellets_totais, sizeof(int), 1, fp);
    fwrite(&mapa->frame_counter, sizeof(int), 1, fp);
    fwrite(&mapa->numero_fantasmas, sizeof(int),1,fp);
    fwrite(mapa->level, sizeof(int),1,fp);


    for (int i = 0; i < TAM_I; i++) {
        if (mapa->grid_mapa && mapa->grid_mapa[i]) {
            fwrite(mapa->grid_mapa[i], sizeof(char), TAM_J, fp);
        }
    }

    for (int i = 0; i < TAM_I; i++) {
    
        if (mapa->matriz_auxiliar && mapa->matriz_auxiliar[i]) {
            fwrite(mapa->matriz_auxiliar[i], sizeof(int), TAM_J, fp);
        }
    }

    if (mapa->mapa_mascaras) {
        fwrite(mapa->mapa_mascaras, sizeof(int), MASK_SIZE, fp); 
    }
}


/**
 * @brief Escreve os dados do jogo em um arquivo binário
 * @param path Caminho do arquivo binário
 * @param pacman Ponteiro para a estrutura do jogador
 * @param ghost Ponteiro para o array de inimigos
 * @param map Ponteiro para a estrutura do mapa
 */
void writeToBin(char* path, tJogador* pacman, tInimigo* ghost, tMapa* map){
    FILE* arq;
    arq = fopen(path, "wb+");
    if(arq == NULL){
        puts("[Save] erro de abertura\n");
        return;
    }
    save_map(map, arq);
    save_player(pacman, arq);
    save_fantasma(ghost, *map, arq);

    fclose(arq);
    return;
}


/**
 * @brief Carrega os dados do mapa de um arquivo binário
 * @param mapa Ponteiro para a estrutura do mapa
 * @param fp Ponteiro para o arquivo binário
 * @param fantasma Ponteiro para o array de inimigos
 * @param assets Assets do jogo
 */
void load_map(tMapa* mapa, FILE* fp, tInimigo** fantasma, tAssets assets) {
    
    if (!mapa->grid_mapa || !mapa->matriz_auxiliar || !mapa->mapa_mascaras) {
        return; 
    }
    int numero_fantasmas_antigo = mapa->numero_fantasmas;
    fread(&mapa->spritesheet, sizeof(Rectangle), 1, fp);
    fread(&mapa->tamanho_spritesheet, sizeof(int), 1, fp);
    fread(&mapa->pellets_totais, sizeof(int), 1, fp);
    fread(&mapa->frame_counter, sizeof(int), 1, fp);
    fread(&mapa->numero_fantasmas, sizeof(int),1,fp);
    fread(mapa->level, sizeof(int),1,fp);

    //se difere o numero de fantasmas
    if(numero_fantasmas_antigo != mapa->numero_fantasmas)
    {
        *fantasma = realloc(*fantasma, sizeof(tInimigo)*mapa->numero_fantasmas);
        //correcao de sprites pra caso haja um ou mais fantasmas adicionais
        criaNovoFantasma(fantasma, assets, *mapa);
    }

    for (int i = 0; i < TAM_I; i++) {
        fread(mapa->grid_mapa[i], sizeof(char), TAM_J, fp);
    }

    for (int i = 0; i < TAM_I; i++) {
        fread(mapa->matriz_auxiliar[i], sizeof(int), TAM_J, fp);
    }
    
    fread(mapa->mapa_mascaras, sizeof(int), MASK_SIZE, fp);
}

/**
 * @brief Carrega os dados do jogador de um arquivo binário
 * @param pacman Ponteiro para a estrutura do jogador
 * @param arq Ponteiro para o arquivo binário
 */
void load_player(tJogador* pacman, FILE* arq)
{
    //fread(pacman, sizeof(tJogador), 1, arq);
    fread(&pacman->pos.x, sizeof(float), 1, arq);
    fread(&pacman->pos.y, sizeof(float), 1, arq);
    fread(&pacman->dir, sizeof(int), 1, arq);
    fread(&pacman->move_x, sizeof(int), 1, arq);
    fread(&pacman->move_y,sizeof(int), 1, arq);
    fread(&pacman->spd, sizeof(int), 1, arq);
    fread(&pacman->power_pellet, sizeof(bool), 1, arq);
    fread(&pacman->score, sizeof(int), 1, arq);
    fread(&pacman->vida, sizeof(int), 1, arq); 
    fread(&pacman->remainingPellets, sizeof(int), 1, arq);
    fread(&pacman->desenho, sizeof(bool), 1, arq);
    fread(&pacman->tempo_power_pellet, sizeof(float), 1, arq);
    fread(&pacman->current_fruit, sizeof(int), 1, arq);
    fread(&pacman->comendo.rotacao, sizeof(int),1, arq);
    fread(&pacman->comendo.spritesheet.width, sizeof(int), 1, arq);
    fread(&pacman->fruta_ativa, sizeof(FruitType), 1, arq);
    fread(&pacman->curFruitTimer, sizeof(float), 1, arq);
    fread(&pacman->current_fruit, sizeof(int), 1, arq);
    //att da pos da animacao pra sincronizar os bgl
    pacman->comendo.pos.x = pacman->pos.x;
    pacman->comendo.pos.y = pacman->pos.y;
}

/**
 * @brief Carrega os dados dos inimigos de um arquivo binário
 * @param fantasma Ponteiro para o array de inimigos
 * @param map Mapa do jogo
 * @param arq Ponteiro para o arquivo binário
 */
void load_fantasma(tInimigo* fantasma, tMapa map, FILE* arq)
{   
    //realoca dependendo do numero de fantasmas do mapa que sera carregado
    if(fantasma == NULL)
    {
        puts("[LOAD] erro de carregamento de fantasmas\n");
        return;
    }
    for(int i = 0; i < map.numero_fantasmas; i++)
    {
        fread(&fantasma[i].spd, sizeof(float), 1, arq);
        fread(&fantasma[i].vulneravel, sizeof(bool), 1, arq);;
        fread(&fantasma[i].direcao, sizeof(int), 1, arq);;
        fread(&fantasma[i].tempo_morto, sizeof(float), 1, arq);; 
        fread(&fantasma[i].pos, sizeof(Vector2), 1, arq);
        fread(&fantasma[i].pos_inicial, sizeof(Vector2), 1, arq);
        fread(&fantasma[i].type, sizeof(int), 1, arq);
        fread(&fantasma[i].desenho, sizeof(bool), 1, arq);
        fread(&fantasma[i].morte.contador, sizeof(float), 1, arq);
        fread(&fantasma[i].morto.contador, sizeof(float), 1, arq);
        fread(&fantasma[i].colisao_fantasma, sizeof(Rectangle),1,arq);
    }
}

/**
 * @brief Lê os dados do jogo de um arquivo binário
 * @param path Caminho do arquivo binário
 * @param pacman Ponteiro para a estrutura do jogador
 * @param ghosts Ponteiro para o array de inimigos
 * @param map Ponteiro para a estrutura do mapa
 * @param assets Assets do jogo
 */
void readFromBin(char* path, tJogador* pacman, tInimigo** ghosts, tMapa* map, tAssets assets){
    FILE* arq;
    arq = fopen(path, "rb");
    if(arq == NULL){
        puts("[LOAD] erro de abertura\n");
        return;
    }

    load_map(map, arq, ghosts, assets);
    load_player(pacman, arq);
    load_fantasma(*ghosts, *map, arq);

    fclose(arq);
    return;
}

/**
 * @brief Salva o estado atual do jogo em um slot específico
 * @param pacman Ponteiro para a estrutura do jogador
 * @param ghosts Ponteiro para o array de inimigos
 * @param map Ponteiro para a estrutura do mapa
 */
void save(tJogador* pacman, tInimigo* ghosts, tMapa* map) {
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", 1);
    writeToBin(path, pacman, ghosts, map);
}

/**
 * @brief Carrega o estado do jogo de um slot específico
 * @param pacman Ponteiro para a estrutura do jogador
 * @param ghosts Ponteiro para o array de inimigos
 * @param map Ponteiro para a estrutura do mapa
 * @param assets Assets do jogo
 */
void load(tJogador* pacman, tInimigo** ghosts, tMapa* map, tAssets assets){
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", 1);
    readFromBin(path, pacman, ghosts, map, assets);
    return;
}