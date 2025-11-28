#pragma once
#include "../header.h"

typedef enum {
    MENU_START,
    MENU_LOAD,
    MENU_QUIT
} MainMenuOptions;

const char* mainMenuText[] = {
    "Iniciar Jogo",
    "Carregar Jogo",
    "Sair"
};
static int menuIndex = 0;
static void drawMainMenu(void){
    ClearBackground(BLACK);
    const char* title = "PAC-MENOS";
    int titleW = MeasureText(title, 90);
    DrawText(title, (LARGURA - titleW)/2, 120, 90, YELLOW);
    int startY = 300;
    for(int i = MENU_START; i <= MENU_QUIT; i++){
        int y = startY + i * 80;
        if(i == menuIndex){
            DrawRectangle((LARGURA/2) - 170, y - 10, 340, 60, LIGHTGRAY);
            DrawText(">", (LARGURA/2) - 210, y + 5, 40, WHITE);
            DrawText(mainMenuText[i], (LARGURA/2) - 140, y + 5, 40, BLACK);
        } 
        else {
            DrawRectangle((LARGURA/2) - 160, y, 320, 50, DARKGRAY);
            DrawText(mainMenuText[i], (LARGURA/2) - 140, y + 5, 35, WHITE);
        }
    }
}
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

void mainMenu(void){
    menuIndex = 0;
    while(true){
        BeginDrawing();
        drawMainMenu();
        EndDrawing();
        int choice = mainMenuInputs();
        if(choice == -1) continue;
        switch(choice){
            case MENU_START:
                return;
            case MENU_LOAD:
                break;
            case MENU_QUIT:
                CloseWindow();
                exit(0);
                break;
        }
    }
}