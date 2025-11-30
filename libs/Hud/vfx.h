#include "raylib.h"
#include "../Character/player.h"

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
        DrawText(TextFormat("+%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y, 20, Fade(RED, vfx->tempo_restante));
    }
    else
    {
        DrawText(TextFormat("%d", vfx->parametro_especial), vfx->pos.x, vfx->pos.y, 20, Fade(RED, vfx->tempo_restante));
    }
    //reset da funcao
    if(vfx->tempo_restante <= 0)
    {
        vfx->tempo_restante = vfx->tempo;
        vfx->ativo = false;
    }
}