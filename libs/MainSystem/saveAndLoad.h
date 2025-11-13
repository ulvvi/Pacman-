#pragma once
#include "../header.h"
#include "../Character/fantasma.h"
#include "../MainSystem/system.h"
#include "../Character/player.h"


void writeToBin(char* path, tJogador* pacman, tInimigo* ghost, tMapa* map){
    FILE* arq;
    arq = fopen(path, "wb+");
    if(arq == NULL){
        puts("[Save] erro de abertura\n");
        return;
    }
    fwrite(pacman, sizeof(tJogador), 1, arq);
    for(int i = 0; i < calculaFantasmas(map->grid_mapa); i++){
        fwrite(&ghost[i], sizeof(tInimigo), 1, arq);
    } 
    fwrite(map, sizeof(tMapa), 1, arq);
    fclose(arq);
    return;
}

void readFromBin(char* path, tJogador* pacman, tInimigo* ghosts, tMapa* map){
    FILE* arq;
    arq = fopen(path, "rb");
    if(arq == NULL){
        puts("[LOAD] erro de abertura\n");
        return;
    }

    fread(pacman, sizeof(tJogador), 1, arq);
    for(int i = 0; i < calculaFantasmas(map->grid_mapa); i++){
        fread(&ghosts[i], sizeof(tInimigo), 1, arq);
    }
    fread(map, sizeof(tMapa), 1, arq);

    fclose(arq);
    return;
}


void save(tJogador* pacman, tInimigo* ghosts, tMapa* map, int slot) {
    char path[256];
    snprintf(path, sizeof(path), "../../saves/save%d.bin", slot);

    writeToBin(path, pacman, ghosts, map);
}

void load(tJogador* pacman, tInimigo* ghosts, tMapa* map, int slot){
    char path[256];
    snprintf(path, sizeof(path), "../../saves/save%d.bin", slot);

    readFromBin(path, pacman, ghosts, map);
    return;
}