#pragma once
#include "../header.h"

Color CYAN = {0, 255, 255, 255};

enum menuOptions{
  BACK,
  NEW,
  SAVE,
  LOAD,
  QUIT
};

void drawMenu(int* index){
  DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.8f));
  DrawText("PAUSE", 10, 10, 40, YELLOW);
  DrawText("Aperte V para voltar", 10, 45, 20, YELLOW);
  DrawText("N: Novo Jogo", 10, 100, 20, PINK);
  DrawText("C: Carregar", 10, 150, 20, ORANGE);
  DrawText("S: Salvar", 10, 200, 20, CYAN);
  DrawText("Q: Sair", 10, 250, 20, RED);
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

void menuInputs(int* index, GameState* state_atual, char** grid_mapa){
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
}

void menuLogic(int* index, GameState* state_atual, char** grid_mapa){
    drawMenu(index);
    printf("%d", *index);
    menuInputs(index, state_atual, grid_mapa);
}
