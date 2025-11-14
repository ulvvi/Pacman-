#pragma once
#include "../header.h"
#include "../Character/fantasma.h"
#include "../MainSystem/system.h"
#include "../Character/player.h"



void save_map(tMapa* mapa, FILE* fp) {

    fwrite(&mapa->tileset_parede, sizeof(Texture2D), 1, fp);
    fwrite(&mapa->spritesheet, sizeof(Rectangle), 1, fp);
    fwrite(&mapa->tamanho_spritesheet, sizeof(int), 1, fp);
    fwrite(&mapa->pellets_totais, sizeof(int), 1, fp);

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
    fwrite(pacman, sizeof(tJogador), 1, arq);
    for(int i = 0; i < calculaFantasmas(map->grid_mapa); i++){
        fwrite(&ghost[i], sizeof(tInimigo), 1, arq);
    }
    fclose(arq);
    return;
}

void load_map(tMapa* mapa, FILE* fp) {
    
    if (!mapa->grid_mapa || !mapa->matriz_auxiliar || !mapa->mapa_mascaras) {
        return; 
    }

    fread(&mapa->tileset_parede, sizeof(Texture2D), 1, fp); 
    fread(&mapa->spritesheet, sizeof(Rectangle), 1, fp);
    fread(&mapa->tamanho_spritesheet, sizeof(int), 1, fp);
    fread(&mapa->pellets_totais, sizeof(int), 1, fp);

    for (int i = 0; i < TAM_I; i++) {

        mapa->grid_mapa[i] = (char*)malloc(TAM_J * sizeof(char));
        fread(mapa->grid_mapa[i], sizeof(char), TAM_J, fp);
    }

    for (int i = 0; i < TAM_I; i++) {

        mapa->matriz_auxiliar[i] = (int*)malloc(TAM_J * sizeof(int));
        fread(mapa->matriz_auxiliar[i], sizeof(int), TAM_J, fp);
    }
    
    fread(mapa->mapa_mascaras, sizeof(int), MASK_SIZE, fp);
}

void readFromBin(char* path, tJogador* pacman, tInimigo* ghosts, tMapa* map){
    FILE* arq;
    arq = fopen(path, "rb");
    if(arq == NULL){
        puts("[LOAD] erro de abertura\n");
        return;
    }

    load_map(map, arq);

    fread(pacman, sizeof(tJogador), 1, arq);

    for(int i = 0; i < calculaFantasmas(map->grid_mapa); i++){
        fread(&ghosts[i], sizeof(tInimigo), 1, arq);
        alinhaFantasmaAoGrid(&ghosts[i]);
    }

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