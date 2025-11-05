#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"

#pragma once

void initiateAudio(Music stems[], int level){

    InitAudioDevice();

    switch(level){
        case 1:
            stems[0] = LoadMusicStream("audio/ambiente/teste.wav");
            stems[1] = LoadMusicStream("audio/ambiente/menu_theme.wav");
            stems[2] = LoadMusicStream("audio/ambiente/pellet.wav");
        break;
    }
    return;
}


void drawBackground(char** grid_mapa, Texture2D tileset_parede, Rectangle spritesheet, tJogador pacman, GameState state_atual, int** matriz_auxiliar){
    //layer fundo/mapa   
    BeginDrawing(); 
    ClearBackground(BLACK);
    drawMap(grid_mapa);
    drawTexturaParede(matriz_auxiliar, tileset_parede, spritesheet);
    //layer entidades
    DrawRectangle(pacman.pos.x, pacman.pos.y, TAM_GRID, TAM_GRID, YELLOW);
    //layer main HUD
    drawHUD(pacman.score, pacman.remainingPellets);
    DrawText(TextFormat("posx: %.2f, posy: %.2f", pacman.pos.x, pacman.pos.y), 900, 810, 20, WHITE);
}


void updateLogic(tJogador* pacman, char** grid_mapa, int* grid_i, int* grid_j, GameState* state_atual, int option){

    if(IsKeyPressed(KEY_TAB))
    {
        option = 0;
        *state_atual = PAUSE;
    }

    //movimentacao
    movePlayer(grid_mapa, pacman, grid_i, grid_j);

    //colisoes pellets
    if(checaPlayerCentralizado(pacman))
    {   
        colisaoPellets(pacman, grid_mapa, &pacman->score, &pacman->remainingPellets, *grid_i, *grid_j);
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
}