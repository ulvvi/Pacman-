//HUD.h
#pragma once
#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include "system.h"


void cutIn(void)
{
    Sound som_cut_in = LoadSound("audio/ambiente/cut_in.mp3");
    Texture2D cut_in = LoadTexture("sprites/player/pacman_cut_in.png");
    static int contador = 0;
    PlaySound(som_cut_in);
    while((float)contador/60 < 1.5)
    {
        BeginDrawing();
        DrawTexture(cut_in, 0, ALTURA/2 - 640/2, WHITE);
        EndDrawing();
        contador++;
    }
    contador = 0;
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
}

void drawHUD(int score, int totalPellets){
    DrawRectangle(0, 800, 1600, 40, BLACK); //desenho da HUD
    DrawText(TextFormat("Score: %d", score), 10, 810, 20, WHITE);
    DrawText(TextFormat("pellets: %d", totalPellets), 150, 810, 20, WHITE);
}

void gameOver(void)
{
    int tam_over = 80;
    int tam_resto = 20;
    char texto_over[] = {"FIM DE JOGO"};
    char texto_menu[] = {"V para retornar ao MENU"};
    char texto_sair[] = {"ESC para sair do jogo"};
    bool game_over = true;
    while(game_over == true)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("FIM DE JOGO", (LARGURA - MeasureText(texto_over, tam_over))/2, ALTURA/2 - tam_over/2, 80, RED);
        DrawText("V para retornar ao MENU", (LARGURA - MeasureText(texto_menu, tam_resto))/2, ALTURA/2 + tam_resto*3, tam_resto, YELLOW);
        DrawText("ESC para sair do jogo", (LARGURA - MeasureText(texto_sair, tam_resto))/2, ALTURA/2 + tam_resto*5, tam_resto, YELLOW);
        EndDrawing();

        if(IsKeyPressed(KEY_V))
        {
            //voltar ao menu de alguma forma
            game_over = false;
        }
        if(IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
            exit(0);
        }
    }
}
