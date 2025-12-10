#pragma once
#include "../header.h"

typedef enum {
    MENU_START,
    MENU_LOAD,
    MENU_QUIT
} MainMenuOptions;

const char* mainMenuText[] = {
    "Começar",
    "Carregar Jogo",
    "Sair"
};

static int menuIndex = 0;

/**
 * @brief Desenha o menu principal na tela.
 */
static void drawMainMenu(void){
    ClearBackground(BLACK);
    const char* title = "PACMAN+";
    int titleW = MeasureText(title, 90);
    DrawText(title, (LARGURA - titleW)/2, 120, 90, PEL);
    int startY = 300;
    for(int i = MENU_START; i <= MENU_QUIT; i++){
        int y = startY + i * 80;
        if(i == menuIndex){
            DrawRectangle((LARGURA/2) - 170, y - 10, 340, 60, LIGHTGRAY);
            //DrawText(">", (LARGURA/2) - 210, y + 5, 40, WHITE);
            DrawText(mainMenuText[i], (LARGURA/2) - 140, y + 5, 40, BLACK);
        } 
        else {
            DrawRectangle((LARGURA/2) - 160, y, 320, 50, DARKGRAY);
            DrawText(mainMenuText[i], (LARGURA/2) - 140, y + 5, 35, WHITE);
        }
    }
}

/**
 * @brief Processa as entradas do usuário no menu principal.
 */
static int mainMenuInputs(void){
    if(IsKeyPressed(KEY_DOWN)){
        menuIndex++;
        if(menuIndex > MENU_QUIT) menuIndex = MENU_START;
    }
    if(IsKeyPressed(KEY_UP)){
        menuIndex--;
        if(menuIndex < MENU_START) menuIndex = MENU_QUIT;
    }
    if(IsKeyPressed(KEY_ENTER)){
        return menuIndex;
    }

    return -1;
}

/**
 * @brief Executa o loop do menu principal.
 * @return int A opção selecionada pelo usuário.
 */
int mainMenu(void){
    menuIndex = 0;
    Music theme = LoadMusicStream("audio/Music/title.wav");
    SetMusicVolume(theme, 1.25f);
    PlayMusicStream(theme);

    while(true){
        UpdateMusicStream(theme);
        BeginDrawing();
        drawMainMenu();
        EndDrawing();
        int choice = mainMenuInputs();
        if(choice == -1) continue;
        switch(choice){
            case MENU_START:
                return 1;
            case MENU_LOAD:
                return 2;
                break;
            case MENU_QUIT:
                UnloadMusicStream(theme);
                CloseWindow();
                exit(0);
                break;
        }
    }
    return 0;
}