#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header.h"


void writeToBin(char* path, tJogador* pacman, tInimigo* ghost, tMapa* map){
    FILE* arq;
    arq = fopen(path, "wb+");
    if(arq == NULL){
        puts("[Save] erro de abertura\n");
        return;
    }
    fwrite(pacman, sizeof(tJogador), 1, arq);
    fwrite(ghost, sizeof(tInimigo), 1, arq);
    fwrite(map, sizeof(tMapa), 1, arq);
    fclose(arq);
    return;
}

void readFromBin(char* path, tJogador* pacman, tInimigo* ghost, tMapa* map){
    FILE* arq;
    arq = fopen(path, "rb");
    if(arq == NULL){
        puts("[LOAD] erro de abertura\n");
        return;
    }

    fread(pacman, sizeof(tJogador), 1, arq);
    fread(ghost, sizeof(tInimigo), 1, arq);
    fread(map, sizeof(tMapa), 1, arq);

    fclose(arq);
    return;
}


void save(tJogador* pacman, tInimigo* ghost, tMapa* map, int slot) {
    char path[256];
    snprintf(path, sizeof(path), "../../saves/save%d.bin", slot);

    writeToBin(path, pacman, ghost, map);
}

void load(tJogador* pacman, tInimigo* ghost, tMapa* map, int slot){
    char path[256];
    snprintf(path, sizeof(path), "../../saves/save%d.bin", slot);

    readFromBin(path, pacman, ghost, map);
    return;
}