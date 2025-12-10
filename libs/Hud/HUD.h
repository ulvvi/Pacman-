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
#define COLOR_TITLE_BG            ROXO
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

/**
 * @brief Função que desenha a fruta atual na HUD.
 * @param curFruit Índice da fruta atual.
 * @param mapa Estrutura do mapa contendo as texturas das frutas.
 */
void drawFruit(int curFruit, tMapa mapa){
  DrawRectangle(1400, 800, 40, 40, ROXO);
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

/**
 * @brief Função que desenha a HUD na tela.
 * @param score Pontuação atual do jogador.
 * @param totalPellets Número total de pellets restantes.
 * @param curFruit Índice da fruta atual.
 * @param mapa Estrutura do mapa contendo as texturas das frutas.
 */
void drawHUD(int score, int totalPellets, int curFruit, tMapa mapa, tAssets assets, int vida){
    int pos_vida_x = 1450;
    int pos_vida_y = 800;
    int espacamento = assets.pacman_vida_sprite.width - 10;
    DrawRectangle(0, 800, 1600, 40, BLACK); //desenho da HUD
    DrawText(TextFormat("Score: %d", score), 10, 810, 20, WHITE);
    DrawText(TextFormat("pellets: %d", totalPellets), 150, 810, 20, WHITE);
    drawFruit(curFruit, mapa);
    for(int i = 0; i < vida; i++)
    {
      // DrawRectangle(pos_vida_x + espacamento*i, pos_vida_y, 20, 20, WHITE);
      DrawTexture(assets.pacman_vida_sprite, pos_vida_x + espacamento*i, pos_vida_y, WHITE);
    }
}


/**
 * @brief Função que exibe a tela de game over.
 * @return true se o jogador escolher voltar ao menu, false para tentar novamente.
 */
bool gameOver()
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
        EndDrawing();

        if(IsKeyPressed(KEY_V))
        {
            //voltar ao menu de alguma forma
            return false;
        }
        if(IsKeyPressed(KEY_M))
        {
            //voltar ao menu de alguma forma
            return true;
        }
    }
}

/**
 * @brief Função que desenha o menu de pausa na tela.
 * @param menuData Ponteiro para a estrutura do menu.
 */
void drawMenu(tMenu* menuData){
    DrawRectangle(0, 0, LARGURA, ALTURA, COLOR_BACKGROUND_OVERLAY);

    const char* titleText = "PAUSE";
    int titleWidth = MeasureText(titleText, TITLE_FONT_SIZE);
    int titleY = 100;

    DrawRectangle(MENU_SPACING_X - 5, titleY - 5, titleWidth + 30, TITLE_FONT_SIZE + 30, ROXO_ESCURO);
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
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth + 5, expandedHeight + 5, ROXO_ESCURO);
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth, expandedHeight, COLOR_OPTION_NORMAL_BG);
            DrawText(menuOptionsText[i], MENU_SPACING_X, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_OPTION_NORMAL_TEXT);
              
            }
            else
            {
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth + 5, expandedHeight + 5, ROXO_ESCURO);
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth, expandedHeight, COLOR_OPTION_HIGHLIGHT_BG);
            DrawText(menuOptionsText[i], MENU_SPACING_X, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_OPTION_HIGHLIGHT_TEXT);
            }

            DrawText(">", MENU_SPACING_X - 30, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_SELECTOR);

            
        } 
        else{
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH + 5, OPTION_HEIGHT + 5, ROXO_ESCURO);
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH, OPTION_HEIGHT, COLOR_OPTION_NORMAL_BG);
            
            DrawText(menuOptionsText[i], MENU_SPACING_X + 10, currentY + (OPTION_HEIGHT - 20) / 2, OPTION_FONT_SIZE - 10, COLOR_OPTION_NORMAL_TEXT);
        }
    }
}

/**
 * @brief Função que verifica qual opção do menu foi selecionada.
 * @param menuData Ponteiro para a estrutura do menu.
 * @return Índice da opção selecionada ou -1 se nenhuma foi selecionada.
 */
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

/**
 * @brief Função que processa os inputs do menu de pausa.
 * @param menuData Ponteiro para a estrutura do menu.
 * @param state_atual Ponteiro para o estado atual do jogo.
 * @param map Ponteiro para a estrutura do mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param ghosts Ponteiro duplo para a estrutura dos inimigos.
 * @param assets Estrutura dos assets do jogo.
 * @return -2 para novo jogo, -1 para sair, 0 caso contrário.
 */
int menuInputs(tMenu* menuData, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo** ghosts, tAssets assets){
  if(IsKeyPressed(KEY_V) || currentChosen(menuData) == BACK){
    *state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) || currentChosen(menuData) == NEW){
    return -2;
  }
  if(IsKeyPressed(KEY_Q) || currentChosen(menuData) == QUIT){
    return -1;
  }
  if(IsKeyPressed(KEY_S) || currentChosen(menuData) == SAVE){
    save(pacman, *ghosts, map); 
  }
  if(IsKeyPressed(KEY_L) || currentChosen(menuData) == LOAD){
    load(pacman, ghosts, map, assets);
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
  return 0;
}

/**
 * @brief Função que gerencia a lógica do menu de pausa.
 * @param menuData Ponteiro para a estrutura do menu.
 * @param state_atual Ponteiro para o estado atual do jogo.
 * @param map Ponteiro para a estrutura do mapa.
 * @param pacman Ponteiro para a estrutura do jogador.
 * @param ghosts Ponteiro duplo para a estrutura dos inimigos.
 * @param assets Estrutura dos assets do jogo.
 * @return -2 para novo jogo, -1 para sair, 0 caso contrário.
 */
int menuLogic(tMenu* menuData, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo** ghosts, tAssets assets){
  int subIndex = 0;
  drawMenu(menuData);
  /*if(menuInputs(menuData, state_atual, map, pacman, ghosts) == -1){
    return -1;
  }*/
  return menuInputs(menuData, state_atual, map, pacman, ghosts, assets);
}
