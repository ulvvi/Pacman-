

enum menuOptions{
  BACK,
  SAVE,
  LOAD,
  QUIT
};

void menuLogic(){
  DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.8f));
  DrawText("PAUSE", 10, 10, 40, YELLOW);
  DrawText("Aperte V para voltar", 10, 45, 20, YELLOW);
  DrawText("N: Novo Jogo", 10, 100, 20, PINK);
  DrawText("C: Carregar", 10, 150, 20, ORANGE);
  DrawText("S: Salvar", 10, 200, 20, CYAN);
  DrawText("Q: Sair", 10, 250, 20, RED);
}

void isHighlighted(){
  
}

void menuInputs(){
  if(IsKeyPressed(KEY_V)){
    state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) ||){
    //resetGameState();
  }
  
  if(IsKeyPressed(KEY_Q)){
    CloseWindow();
    freeDiddy(grid_mapa);
    return 0;
  }

}

