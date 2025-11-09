//HUD.h
#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#pragma once
#include "../header.h"

#define MENU_SPACING 100
#define SpaceBetweenOptions 50
#define OPTION_HEIGHT 40
#define OPTION_WIDTH 270

Color CYAN = {0, 255, 255, 255};

enum menuOptions{
  BACK,
  NEW,
  SAVE,
  LOAD,
  QUIT
};

char* menuOptionsText[] = {
  "V - Voltar ao Jogo",
  "N - Novo Jogo",
  "S - Salvar Jogo",
  "L - Carregar Jogo",
  "Q - Sair do Jogo"
};



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


void drawMenu(int* index){
  DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.8f));
  DrawRectangle(MENU_SPACING - 10, 90, MeasureText("PAUSE", 60) + 20, 70, CYAN);
  DrawText("PAUSE", MENU_SPACING, 100, 60, BLACK);

  for(int i = BACK; i <= QUIT; i++){
    if(i == *index){
      DrawText(">", MENU_SPACING - 20, 203 + i * SpaceBetweenOptions, OPTION_HEIGHT, WHITE);
      DrawRectangle(MENU_SPACING, 196 + i * SpaceBetweenOptions, OPTION_WIDTH*1.2, OPTION_HEIGHT*1.2, LIGHTGRAY);
      DrawText(menuOptionsText[i], MENU_SPACING + 10, 206 + i * SpaceBetweenOptions, 30, BLACK);
      continue;
    }
    DrawRectangle(MENU_SPACING, 200 + i * SpaceBetweenOptions, OPTION_WIDTH, OPTION_HEIGHT, DARKGRAY);
    DrawText(menuOptionsText[i], MENU_SPACING + 10, 210 + i * SpaceBetweenOptions, 20, WHITE);
  }

}

int currentChosen(int cur){
  if(IsKeyPressed(KEY_ENTER)){
    return cur;
  }
  else
  {
    return -1;
  }
}

void menuInputs(int* index, GameState* state_atual, char** grid_mapa, Sound menuClick){
  if(IsKeyPressed(KEY_V) || currentChosen(*index) == BACK){
    *state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) || currentChosen(*index) == NEW){
    //resetGameState();
  }
  if(IsKeyPressed(KEY_Q) || currentChosen(*index) == QUIT){
    CloseWindow();
    return;
  }
  if(IsKeyPressed(KEY_S) || currentChosen(*index) == SAVE){
    //saveGameState(grid_mapa);
  }

  //verifica o input para mudar a opcao selecionada
  if(IsKeyPressed(KEY_DOWN)){
    PlaySound(menuClick);
    *index += 1;
    if(*index > QUIT){
      *index = BACK;
    }
  }
  if(IsKeyPressed(KEY_UP)){
    PlaySound(menuClick);
    *index -= 1;
    if(*index < BACK){
      *index = QUIT;
    }
  }
}

void menuLogic(int* index, GameState* state_atual, char** grid_mapa, Sound menuClick){
    drawMenu(index);
    menuInputs(index, state_atual, grid_mapa, menuClick);
}
