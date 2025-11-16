//HUD.h
#pragma once
#include "../header.h"
#include "../MainSystem/system.h"
#include "../MainSystem/saveAndLoad.h"


#define MENU_SPACING_X 100 
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


/*
void cutIn(Sound som_cut_in, Texture2D cut_in)
{
    DrawTexture(cut_in, 0, ALTURA/2 - 600/2, WHITE);
}*/

void drawHUD(int score, int totalPellets){
    DrawRectangle(0, 800, 1600, 40, BLACK); //desenho da HUD
    DrawText(TextFormat("Score: %d", score), 10, 810, 20, WHITE);
    DrawText(TextFormat("pellets: %d", totalPellets), 150, 810, 20, WHITE);
}

//gameOver não ficaria aqui
void gameOver()
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
        
        if(i == *index){
            int expandedWidth = OPTION_WIDTH * 1.1; 
            int expandedHeight = OPTION_HEIGHT * 1.1;
            int xOffset = (expandedWidth - OPTION_WIDTH) / 2;
            int yOffset = (expandedHeight - OPTION_HEIGHT) / 2;

            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth + 5, expandedHeight + 5, DARKGRAY);
            DrawRectangle(MENU_SPACING_X - xOffset, currentY - yOffset, expandedWidth, expandedHeight, COLOR_OPTION_HIGHLIGHT_BG);


            DrawText(menuOptionsText[i], MENU_SPACING_X, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_OPTION_HIGHLIGHT_TEXT);
            
            DrawText(">", MENU_SPACING_X - 30, currentY + (expandedHeight - OPTION_FONT_SIZE) / 2, OPTION_FONT_SIZE, COLOR_SELECTOR);
            
        } 
        else{
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH + 5, OPTION_HEIGHT + 5, DARKGRAY);
            DrawRectangle(MENU_SPACING_X, currentY, OPTION_WIDTH, OPTION_HEIGHT, COLOR_OPTION_NORMAL_BG);
            
            DrawText(menuOptionsText[i], MENU_SPACING_X + 10, currentY + (OPTION_HEIGHT - 20) / 2, OPTION_FONT_SIZE - 10, COLOR_OPTION_NORMAL_TEXT);
        }
    }
}

int currentChosen(int cur, Sound sfx[]){
  if(IsKeyPressed(KEY_ENTER)){
    PlaySound(sfx[1]);
    return cur;
  }
  else
  {
    return -1;
  }
}

void menuInputs(int* index, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo* ghosts, Sound sfx[]){
  if(IsKeyPressed(KEY_V) || currentChosen(*index, sfx) == BACK){
    *state_atual = GAMEPLAY;
  }
  if(IsKeyPressed(KEY_N) || currentChosen(*index, sfx) == NEW){

  }
  if(IsKeyPressed(KEY_Q) || currentChosen(*index, sfx) == QUIT){
    CloseWindow();
    return;
  }
  if(IsKeyPressed(KEY_S) || currentChosen(*index, sfx) == SAVE){
    save(pacman, ghosts, map, 1); //salva no slot 1 por enquanto
  }
  if(IsKeyPressed(KEY_L) || currentChosen(*index, sfx) == LOAD){
    load(pacman, ghosts, map, 1); //carrega do slot 1 por enquanto
  }

  //verifica o input para mudar a opcao selecionada
  if(IsKeyPressed(KEY_DOWN)){
    PlaySound(sfx[0]);
    *index += 1;
    if(*index > QUIT){
      *index = BACK;
    }
  }
  if(IsKeyPressed(KEY_UP)){
    PlaySound(sfx[0]);
    *index -= 1;
    if(*index < BACK){
      *index = QUIT;
    }
  }
}

void menuLogic(int* index, GameState* state_atual, tMapa* map, tJogador* pacman, tInimigo* ghosts, Sound sfx[]){
    drawMenu(index);
    menuInputs(index, state_atual, map, pacman, ghosts, sfx);
}
