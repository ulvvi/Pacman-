//HUD.h
#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#include "../MainSystem/saveAndLoad.h"


#define MENU_SPACING_X 100
#define SUBMENU_SPACING_X  MENU_SPACING_X + 100  
#define MENU_PADDING_Y 20  
#define OPTION_HEIGHT 40
#define OPTION_WIDTH 300
#define TITLE_FONT_SIZE 60
#define OPTION_FONT_SIZE 30
#define OPTION_COUNT 5

#define COLOR_BACKGROUND_OVERLAY  Fade(BLACK, 0.8f)
#define COLOR_TITLE_BG            BLUE
#define COLOR_TITLE_TEXT          RAYWHITE
#define COLOR_OPTION_NORMAL_BG    BLACK
#define COLOR_OPTION_NORMAL_TEXT  RAYWHITE
#define COLOR_OPTION_HIGHLIGHT_BG LIGHTGRAY
#define COLOR_OPTION_HIGHLIGHT_TEXT BLACK
#define COLOR_SELECTOR            RAYWHITE

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

char* submenuOptionsText[] = {
  "1 - Slot 1",
  "2 - Slot 2",
  "3 - Slot 3",
  "4 - Slot 4",
  "5 - Slot 5"
};


void drawFruit(int curFruit, tMapa mapa){
  DrawRectangle(1400, 800, 40, 40, DARKBLUE);
  DrawRectangle(1405, 805, 30, 30, BLACK);

  switch(curFruit){
    case 0:
      DrawTexture(mapa.cherry,1400, 800, WHITE);
      break;
    case 1:
      DrawTexture(mapa.strawberry,1400, 800, WHITE);
      break;
    case 2:
      DrawTexture(mapa.grape,1400, 800, WHITE);
      break;
    case 3:
      DrawTexture(mapa.blueberry,1400, 800, WHITE);
      break;
  }
}

void drawHUD(int score, int totalPellets, int curFruit, tMapa mapa){
    DrawRectangle(0, 800, 1600, 40, BLACK); //desenho da HUD
    DrawText(TextFormat("Score: %d", score), 10, 810, 20, WHITE);
    DrawText(TextFormat("pellets: %d", totalPellets), 150, 810, 20, WHITE);
    drawFruit(curFruit, mapa);
}


/*a ideia agr do game over é ele retornar um valor e na main a gnt fazer algo dependendo do retorno. como gameover so precisa ter o voltar pro menu, rejogar
fase e fechar jogo, n vai encher a main de coisa n, eh de boa*/

int gameOver()
{
    int tam_over = 80;
    int tam_resto = 20;
    char texto_over[] = {"FIM DE JOGO"};
    char texto_menu[] = {"V para tentar novamente"};
    char texto_sair[] = {"ESC para sair do jogo"};
    bool game_over = true;
    while(true)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("FIM DE JOGO", (LARGURA - MeasureText(texto_over, tam_over))/2, ALTURA/2 - tam_over/2, 80, RED);
        DrawText("V para rejogar a fase", (LARGURA - MeasureText(texto_menu, tam_resto))/2, ALTURA/2 + tam_resto*3, tam_resto, YELLOW);
        DrawText("M para ir ao menu", (LARGURA - MeasureText(texto_menu, tam_resto))/2, ALTURA/2 + tam_resto*5, tam_resto, YELLOW);
        DrawText("ESC para sair do jogo", (LARGURA - MeasureText(texto_sair, tam_resto))/2, ALTURA/2 + tam_resto*7, tam_resto, YELLOW);
        EndDrawing();

        if(IsKeyPressed(KEY_V))
        {
            //voltar ao menu de alguma forma
            return 0;
        }
        if(IsKeyPressed(KEY_M))
        {
            //voltar ao menu de alguma forma
            return 1;
        }
        if(IsKeyPressed(KEY_ESCAPE))
        {
            return 2;
        }
    }
}


void drawMenu(tMenu* menuData){
    DrawRectangle(0, 0, LARGURA, ALTURA, COLOR_BACKGROUND_OVERLAY);

    const char* titleText = "PAUSE";
    int titleWidth = MeasureText(titleText, TITLE_FONT_SIZE);
    int titleY = 100;

    DrawRectangle(MENU_SPACING_X - 5, titleY - 5, titleWidth + 30, TITLE_FONT_SIZE + 30, DARKBLUE);
    DrawRectangle(MENU_SPACING_X - 10, titleY - 10, titleWidth + 20, TITLE_FONT_SIZE + 20, COLOR_TITLE_BG);

    DrawText(titleText, MENU_SPACING_X, titleY, TITLE_FONT_SIZE, COLOR_TITLE_TEXT);


    int startY = titleY + TITLE_FONT_SIZE + 50; 
    
    for(int i = BACK; i <= QUIT; i++){

        int currentY = startY + i * (OPTION_HEIGHT + MENU_PADDING_Y);
        
        if(i == menuData->index){
            int expandedWidth = OPTION_WIDTH * 1.1; 
            int expandedHeight = OPTION_HEIGHT * 1.1;
            int xOffset = (expandedWidth - OPTION_WIDTH) / 2;
            int yOffset = (expandedHeight - OPTION_HEIGHT) / 2;

            if(IsKeyPressed(KEY_ENTER)){
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth + 5, expandedHeight + 5, DARKGRAY);
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth, expandedHeight, COLOR_OPTION_NORMAL_BG);
            DrawText(menuOptionsText[i], MENU_SPACING_X, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_OPTION_NORMAL_TEXT);
              
            }
            else
            {
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth + 5, expandedHeight + 5, DARKGRAY);
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth, expandedHeight, COLOR_OPTION_HIGHLIGHT_BG);
            DrawText(menuOptionsText[i], MENU_SPACING_X, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_OPTION_HIGHLIGHT_TEXT);
            }

            DrawText(">", MENU_SPACING_X - 30, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_SELECTOR);

            
        } 
        else{
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH + 5, OPTION_HEIGHT + 5, DARKGRAY);
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH, OPTION_HEIGHT, COLOR_OPTION_NORMAL_BG);
            
            DrawText(menuOptionsText[i], MENU_SPACING_X + 10, currentY + (OPTION_HEIGHT - 20) / 2, OPTION_FONT_SIZE - 10, COLOR_OPTION_NORMAL_TEXT);
        }
    }
}

int currentChosen(tMenu* menuData){
  if(IsKeyPressed(KEY_ENTER)){
    PlaySound(menuData->menuSFX[1]);
    return menuData->index;
  }
  else
  {
    return -1;
  }
}

void menuInputs(tMenu* menuData, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo* ghosts){
  if(IsKeyPressed(KEY_V) || currentChosen(menuData) == BACK){
    *state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) || currentChosen(menuData) == NEW){

  }
  if(IsKeyPressed(KEY_Q) || currentChosen(menuData) == QUIT){
    CloseWindow();
    return;
  }
  if(IsKeyPressed(KEY_S) || currentChosen(menuData) == SAVE){
    save(pacman, ghosts, map, 1); //salva no slot 1 por enquanto
  }
  if(IsKeyPressed(KEY_L) || currentChosen(menuData) == LOAD){
    load(pacman, ghosts, map, 1); //carrega do slot 1 por enquanto
  }

  //verifica o input para mudar a opcao selecionada
  if(IsKeyPressed(KEY_DOWN)){
    PlaySound(menuData->menuSFX[0]);
    menuData->index += 1;
    if(menuData->index > QUIT){
      menuData->index = BACK;
    }
  }
  if(IsKeyPressed(KEY_UP)){
    PlaySound(menuData->menuSFX[0]);
    menuData->index -= 1;
    if(menuData->index < BACK){
      menuData->index = QUIT;
    }
  }
}

void menuLogic(tMenu* menuData, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo* ghosts){
  int subIndex = 0;
  drawMenu(menuData);
  menuInputs(menuData, state_atual, map, pacman, ghosts);
}
