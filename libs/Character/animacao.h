#include "../MainSystem/system.h"
#define CUT_IN_COLUNAS 5
typedef struct
{
    int frame_atual;
    int total_frames;
    float tempo_frame;
    float contador;
    Texture2D sprite;
    Rectangle spritesheet;
    Vector2 pos;

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
    int colunas_totais = objeto->sprite.width/objeto->spritesheet.width;
    int linhas_totais = objeto->sprite.height/objeto->spritesheet.height;
    static int coluna_atual = 0;
    static int linha_atual = 0;


    if(objeto->contador > objeto->tempo_frame)
    {
        objeto->frame_atual++;
        objeto->contador = 0;
        coluna_atual++;
        if(coluna_atual == colunas_totais)
        {
            coluna_atual = 0;
            linha_atual++;
        }
    }   

    //fim da funcao
    if(objeto->frame_atual == objeto->total_frames)
    {
        *state_atual = state_final;
        objeto->frame_atual = 0;
        objeto->contador = 0;
        coluna_atual = 0;
        linha_atual = 0;
        return;
    }


    objeto->spritesheet.x = objeto->spritesheet.width*coluna_atual;
    objeto->spritesheet.y = objeto->spritesheet.height*linha_atual;
    DrawTextureRec(objeto->sprite, objeto->spritesheet, objeto->pos, WHITE);
    objeto->contador+= GetFrameTime();
    
    
}

