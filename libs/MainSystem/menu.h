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
