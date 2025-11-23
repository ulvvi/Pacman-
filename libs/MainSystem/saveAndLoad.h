#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#include "../Character/player.h"


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

void load_map(tMapa* mapa, FILE* fp) {
    
    if (!mapa->grid_mapa || !mapa->matriz_auxiliar || !mapa->mapa_mascaras) {
        return; 
    }

    //fread(&mapa->tileset_parede, sizeof(Texture2D), 1, fp); 
    fread(&mapa->spritesheet, sizeof(Rectangle), 1, fp);
    fread(&mapa->tamanho_spritesheet, sizeof(int), 1, fp);
    fread(&mapa->pellets_totais, sizeof(int), 1, fp);
    fread(&mapa->frame_counter, sizeof(int), 1, fp);
    fread(&mapa->numero_fantasmas, sizeof(int),1,fp);
    fread(mapa->level, sizeof(int),1,fp);
    

    for (int i = 0; i < TAM_I; i++) {

        //mapa->grid_mapa[i] = (char*)malloc(TAM_J * sizeof(char));
        fread(mapa->grid_mapa[i], sizeof(char), TAM_J, fp);
    }

    for (int i = 0; i < TAM_I; i++) {

        //mapa->matriz_auxiliar[i] = (int*)malloc(TAM_J * sizeof(int));
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
    fantasma = realloc(fantasma, sizeof(tInimigo)*map.numero_fantasmas);
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
    }
}

void readFromBin(char* path, tJogador* pacman, tInimigo* ghosts, tMapa* map){
    FILE* arq;
    arq = fopen(path, "rb");
    if(arq == NULL){
        puts("[LOAD] erro de abertura\n");
        return;
    }

    load_map(map, arq);
    load_player(pacman, arq);
    load_fantasma(ghosts, *map, arq);

    fclose(arq);
    return;
}


void save(tJogador* pacman, tInimigo* ghosts, tMapa* map, int slot) {
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", slot);
    writeToBin(path, pacman, ghosts, map);
}

void load(tJogador* pacman, tInimigo* ghosts, tMapa* map, int slot){
    char path[256];
    snprintf(path, sizeof(path), "saves/save%d.bin", slot);
    readFromBin(path, pacman, ghosts, map);
    return;
}