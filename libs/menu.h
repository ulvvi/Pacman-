

enum menuOptions{
  BACK,
  NEW,
  SAVE,
  LOAD,
  QUIT
};

void menuLogic(int* index){
  drawMenu(index);
  printf("%d", *index);
  menuInputs(index);
}

drawMenu(int* index){
  DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.8f));
  DrawText("PAUSE", 10, 10, 40, YELLOW);
  DrawText("Aperte V para voltar", 10, 45, 20, YELLOW);
  DrawText("N: Novo Jogo", 10, 100, 20, PINK);
  DrawText("C: Carregar", 10, 150, 20, ORANGE);
  DrawText("S: Salvar", 10, 200, 20, CYAN);
  DrawText("Q: Sair", 10, 250, 20, RED);
}

currentChosen(cur){
  if(IsKeyPressed(KEY_ENTER)){
    return cur;
  }
  else
  {
    return -1;
  }
}

void menuInputs(int* index){
  if(IsKeyPressed(KEY_V) || currentChosen(*index) = BACK){
    *index = 0;
    state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) || currentChosen(*index) == NEW){
    //resetGameState();
  }
  if(IsKeyPressed(KEY_Q) || currentChosen(*index) == QUIT){
    CloseWindow();
    freeDiddy(grid_mapa);
    return 0;
  }
}
