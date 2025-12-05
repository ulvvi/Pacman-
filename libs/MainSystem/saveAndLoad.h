#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#include "../Character/player.h"

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
    }
}

void save_player(tJogador* pacman, FILE* arq)
{
    //fwrite(&pacman, sizeof(tJogador), 1, arq);
    //eh red flag total salvar as texturas, por isso q n pode salvar a struct toda do pacman
    //resumidamente, a texture2d é um ID, que muda a cada execucao do jogo, salvar isso causa comportamento indeterminado
    //alem disso, a textura ja vai ta carregada no momento do load do jogo, ent ta tudo ok
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
}

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
    //att da pos da animacao pra sincronizar os bgl
    pacman->comendo.pos.x = pacman->pos.x;
    pacman->comendo.pos.y = pacman->pos.y;
}

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


void save(tJogador* pacman, tInimigo* ghosts, tMapa* map, int slot) {
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", slot);
    writeToBin(path, pacman, ghosts, map);
}

void load(tJogador* pacman, tInimigo** ghosts, tMapa* map, int slot, tAssets assets){
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", slot);
    readFromBin(path, pacman, ghosts, map, assets);
    return;
}