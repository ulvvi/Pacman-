#pragma once
#include "../MainSystem/system.h"

//animacao de "objetos", sempre q atinge o frame final, recomeca
void animaObjeto(tAnimacao* objeto)
{
    Vector2 origem = {objeto->spritesheet.width/2, objeto->spritesheet.height/2};
    //correcao da posicao em relacao ao pivot origem
    Rectangle dest = {objeto->pos.x + origem.x, objeto->pos.y + origem.y, objeto->spritesheet.width, objeto->spritesheet.height};

    objeto->contador+= GetFrameTime();
    if(objeto->contador > objeto->tempo_frame)
    { 
       objeto->frame_atual++;
       objeto->contador = 0;
    }
    if(objeto->frame_atual > objeto->total_frames)
        objeto->frame_atual = 0;

    objeto->spritesheet.x = objeto->frame_atual*objeto->spritesheet.width;
    DrawTexturePro(objeto->sprite, objeto->spritesheet, dest, origem, objeto->rotacao, WHITE);
}

//animacao de "cenas", troca o state no final(quando acaba o numero de frames de animacao)
void cutscene(tAnimacao* objeto, GameState* state_atual, GameState state_final)
{
    Vector2 origem = {abs(objeto->spritesheet.width)/2, objeto->spritesheet.height/2};
    //correcao da posicao em relacao ao pivot origem
    Rectangle dest = {objeto->pos.x + origem.x, objeto->pos.y + origem.y, abs(objeto->spritesheet.width), objeto->spritesheet.height};

    //o spritesheet das cutscenes eu to exportando de forma compacta, por isso, nao é tudo na msm linha e precisa dessas ciusas a mais
    int colunas_totais = objeto->sprite.width/abs(objeto->spritesheet.width);
    int linhas_totais = objeto->sprite.height/abs(objeto->spritesheet.height);
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


    objeto->spritesheet.x = abs(objeto->spritesheet.width)*coluna_atual;
    objeto->spritesheet.y = abs(objeto->spritesheet.height)*linha_atual;
    DrawTexturePro(objeto->sprite, objeto->spritesheet, dest, origem, objeto->rotacao, WHITE);
    //DrawTextureRec(objeto->sprite, objeto->spritesheet, objeto->pos, WHITE);
    objeto->contador+= GetFrameTime();
    
    
}
