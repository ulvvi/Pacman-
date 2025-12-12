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
    DrawRectangle((LARGURA/11), 350, 400, 10, ROXO_ESCURO);
    DrawRectangle((LARGURA - 200), 0, 20, ALTURA, ROXO);
    DrawRectangle(0, 80, LARGURA, 20, ROXO_ESCURO);
    DrawText(title, (LARGURA - titleW)/8, 250, 90, PEL);
    DrawText("Feito por: Pdiddy - Raulzito - Hytalo Santos - Capitão hunter", 0, ALTURA - 40, 20, WHITE);
    int startY = 400;
    for(int i = MENU_START; i <= MENU_QUIT; i++){
        int y = startY + i * 80;
        if(i == menuIndex){
            DrawRectangle((LARGURA/5) - 170, y - 10, 345, 65, ROXO_ESCURO);
            DrawRectangle((LARGURA/5) - 170, y - 10, 340, 60, ROXO);
            //DrawText(">", (LARGURA/2) - 210, y + 5, 40, WHITE);
            DrawText(mainMenuText[i], (LARGURA/5) - 150, y , 40, BLACK);
        } 
        else {
            DrawRectangle((LARGURA/5) - 160, y, 325, 55, ROXO);
            DrawRectangle((LARGURA/5) - 160, y, 320, 50, ROXO_ESCURO);
            DrawText(mainMenuText[i], (LARGURA/5) - 140, y + 10, 35, WHITE);
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
int mainMenu(tAssets assets){
    menuIndex = 0;
    Music theme = LoadMusicStream("audio/Music/title.wav");
    SetMusicVolume(theme, 1.25f);
    PlayMusicStream(theme);
    tAnimacao pacman = {
        0, 4, 0.120, 0, assets.pacman_menu, 
        {0,0,120, 120}, {100, 100}, 0, 0, 0, 1
    };

    tAnimacao frutas = {
        0, 24, 0.100, 0, assets.frutas_menu, 
        {0,0,120, 120}, {100, 100}, 0, 0, 0, 1
    };


    while(true){
        UpdateMusicStream(theme);
        BeginDrawing();
        animaObjeto(&pacman);
        animaObjeto(&frutas);
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