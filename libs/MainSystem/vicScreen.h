#include "../header.h"

/**
 * @brief Função que exibe a tela de vitória.
 * @param obj_vitoria Estrutura de animação para o objeto de vitória.
 * @param obj_confete Estrutura de animação para o objeto de confete.
 */
void victoryScreen(tAnimacao obj_vitoria, tAnimacao obj_confete){
    while(!WindowShouldClose()){
        BeginDrawing();
        char texto_vitoria[] = "VITÓRIA";
        char texto_tecla[] = "Pressione qualquer tecla para continuar";
        int fonte_v = 50, fonte_t = 20;
        int x_v = LARGURA/2 - MeasureText(texto_vitoria, fonte_v)/2, y_v = (ALTURA/2 - fonte_v/2)-60;
        int x_t =  LARGURA/2 - MeasureText(texto_tecla, fonte_t)/2, y_t = (ALTURA/2 - fonte_t/2);
        obj_vitoria.spritesheet.x = 3200;
        obj_vitoria.spritesheet.y = 2520;
        DrawTextureRec(obj_vitoria.sprite, obj_vitoria.spritesheet, (Vector2){0,0}, WHITE);
        animaObjeto(&obj_confete);
        ClearBackground(BLACK);
        DrawText(texto_vitoria, x_v, y_v, fonte_v, pontos_ganhados);
        DrawText(texto_vitoria, x_v+2, y_v+2, fonte_v, sombra_pontos_ganhados);
        DrawText(texto_tecla, x_t, y_t, fonte_t, Fade(WHITE, 0.8));
        EndDrawing();

        
        if(GetKeyPressed() != 0){
            return;
        }
    }
}