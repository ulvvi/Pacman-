#pragma once
#include "../MainSystem/system.h"

/**
 * @brief Função para animar um objeto genérico.
 * @param objeto Ponteiro para a estrutura de animação do objeto.
 */
void animaObjeto(tAnimacao* objeto)
{
    float escala = objeto->escala;
    Vector2 origem = {(objeto->spritesheet.width*escala)/2, (objeto->spritesheet.height*escala)/2};
    //correcao da posicao em relacao ao pivot origem
    Rectangle dest = {objeto->pos.x + origem.x, objeto->pos.y + origem.y, objeto->spritesheet.width*escala, objeto->spritesheet.height*escala};

    objeto->contador+= GetFrameTime();
    if(objeto->contador >= objeto->tempo_frame)
    { 
       objeto->frame_atual++;
       objeto->contador = 0;
    }
    if(objeto->frame_atual >= objeto->total_frames)
        objeto->frame_atual = 0;

    objeto->spritesheet.x = objeto->frame_atual*objeto->spritesheet.width;
    DrawTexturePro(objeto->sprite, objeto->spritesheet, dest, origem, objeto->rotacao, WHITE);
}

/**
 * @brief Função para executar uma cutscene de animação.
 * @param objeto Ponteiro para a estrutura de animação do objeto.
 * @param state_atual Ponteiro para o estado atual do jogo.
 * @param state_final Estado final após a cutscene.
 */
void cutscene(tAnimacao* objeto, GameState* state_atual, GameState state_final)
{
    Vector2 origem = {abs(objeto->spritesheet.width)/2, objeto->spritesheet.height/2};
    //correcao da posicao em relacao ao pivot origem
    Rectangle dest = {objeto->pos.x + origem.x, objeto->pos.y + origem.y, abs(objeto->spritesheet.width), objeto->spritesheet.height};

    //o spritesheet das cutscenes eu to exportando de forma compacta, por isso, nao é tudo na msm linha e precisa dessas ciusas a mais
    int colunas_totais = objeto->sprite.width/abs(objeto->spritesheet.width);
    int linhas_totais = objeto->sprite.height/abs(objeto->spritesheet.height);

    if(objeto->contador > objeto->tempo_frame)
    {
        objeto->frame_atual++;
        objeto->contador = 0;
        objeto->coluna_atual++;
        if(objeto->coluna_atual == colunas_totais)
        {
            objeto->coluna_atual = 0;
            objeto->linha_atual++;
        }
    }   

    //fim da funcao
    if(objeto->frame_atual == objeto->total_frames)
    {
        *state_atual = state_final;
        objeto->frame_atual = 0;
        objeto->contador = 0;
        objeto->coluna_atual = 0;
        objeto->linha_atual = 0;
        return;
    }

    objeto->spritesheet.x = abs(objeto->spritesheet.width)*objeto->coluna_atual;
    objeto->spritesheet.y = abs(objeto->spritesheet.height)*objeto->linha_atual;
    DrawTexturePro(objeto->sprite, objeto->spritesheet, dest, origem, objeto->rotacao, WHITE);
    //DrawTextureRec(objeto->sprite, objeto->spritesheet, objeto->pos, WHITE);
    objeto->contador+= GetFrameTime();
}
