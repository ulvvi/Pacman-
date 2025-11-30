#include "raylib.h"
#include "../Character/player.h"

Color pontos_ganhados = {6, 128, 81, 255};
Color sombra_pontos_ganhados = {17, 96, 97, 255};
Color pontos_perdidos = {179, 45, 125, 255};
Color sombra_pontos_perdidos = {133, 34, 100, 255};
void mostraPontuacao(tVfx* vfx, tJogador pacman)
{
    if(vfx->tempo_restante == vfx->tempo)
    {
        int tam_fonte = 20;
        vfx->pos.x = pacman.pos.x;
        vfx->pos.y = pacman.pos.y;
    }
    //pra pontuacao "flutuar" organicamente
    vfx->pos.x+= (float)GetRandomValue(-25,50)/100;
    vfx->pos.y+= (float)GetRandomValue(25,-50)/100;
    
    vfx->tempo_restante-= GetFrameTime();

    //checagem se pontuacao positiva ou negativa
    if(vfx->parametro_especial > 0)
    {
        DrawText(TextFormat("+%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y+2, 20, Fade(sombra_pontos_ganhados, vfx->tempo_restante));
        DrawText(TextFormat("+%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y, 20, Fade(pontos_ganhados, vfx->tempo_restante));
        
    }
    else
    {
        DrawText(TextFormat("%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y+2, 20, Fade(sombra_pontos_perdidos, vfx->tempo_restante));
        DrawText(TextFormat("%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y, 20, Fade(pontos_perdidos, vfx->tempo_restante));
        
    }
    //sombreamento do texto
   
    //reset da funcao
    if(vfx->tempo_restante <= 0)
    {
        vfx->tempo_restante = vfx->tempo;
        vfx->ativo = false;
    }
}