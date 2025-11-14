#include "../MainSystem/system.h"
typedef struct
{
    int frame_atual;
    int total_frames;
    float tempo_frame;

} tAnimacao;

void animaObjeto(tAnimacao* objeto)
{
    static float contador = 0;
    contador+= GetFrameTime();
    if(contador > objeto->tempo_frame)
        objeto->frame_atual++;
    if(objeto->frame_atual > objeto->total_frames)
        objeto->frame_atual = 0;
}

void cutscene(tAnimacao* objeto, GameState* state_atual, GameState state_final)
{
    static float contador = 0;
    contador+= GetFrameTime();
    if(contador > objeto->tempo_frame)
        objeto->frame_atual++;
    if(objeto->frame_atual > objeto->total_frames)
        *state_atual = state_final;
}