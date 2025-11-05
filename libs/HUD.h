//HUD.h
#pragma once
#include "header.h"
#include "./MainSystem/system.h"

void cutIn(Sound som_cut_in, Texture2D cut_in)
{
    DrawTexture(cut_in, 0, ALTURA/2 - 600/2, WHITE);
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
