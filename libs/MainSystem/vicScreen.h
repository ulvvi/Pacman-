#include "../header.h"

void victoryScreen(){
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("VOCE VENCEU!", 600, 400, 50, GREEN);
        DrawText("Pressione ENTER para continuar", 500, 500, 20, WHITE);
        EndDrawing();

        if(IsKeyPressed(KEY_ENTER)){
            return;
        }
    }
}