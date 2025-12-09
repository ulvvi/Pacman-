#pragma once
#include "system.h"
#include "raylib.h"
#include <math.h>


typedef struct{
    Camera2D camera;
    float duracao;
    float forca;
    bool ativa;
}tCamera;

/**
 * @brief Função que inicializa a câmera com valores padrão.
 * @param camera_relativa Ponteiro para a estrutura da câmera.
 * @param pacman Estrutura do jogador para centralizar a câmera.
 */
void inicializaCamera(tCamera* camera_relativa, tJogador pacman)
{
    camera_relativa->camera.offset.x = (LARGURA)/2;
    camera_relativa->camera.offset.y = (ALTURA)/2;
    camera_relativa->camera.rotation = 0;
    camera_relativa->camera.target.x = (LARGURA)/2;
    camera_relativa->camera.target.y = (ALTURA)/2;
    camera_relativa->camera.zoom = 1;
    camera_relativa->duracao = 0;
    camera_relativa->forca = 0;
    camera_relativa->ativa = false;
}

/**
 * @brief Função que ativa o efeito de tremor de tela.
 * @param camera_relativa Ponteiro para a estrutura da câmera.
 * @param tempo Duração do tremor em segundos.
 */
void ativaCamera(tCamera* camera_relativa, float tempo, float forca)
{
    camera_relativa->duracao = tempo;
    camera_relativa->forca = forca;
    camera_relativa->ativa = true;
}

/**
 * @brief Função que aplica o efeito de tremor de tela na câmera.
 * @param camera_relativa Ponteiro para a estrutura da câmera.
 */
void screenShake(tCamera* camera_relativa)
{
    if(camera_relativa->duracao > 0)
    {
        //essa funcao de bosta so retorna inteiro, entao por isso que to "transformando" em float. em contrapartida
        //o argumento forca tem q ser um numero grande pra dar certo, acima de 100
        float forca_vertical = (GetRandomValue(-camera_relativa->forca, camera_relativa->forca))/100.0f;
        float forca_horizontal = (GetRandomValue(-camera_relativa->forca, camera_relativa->forca))/100.0f;
        //o offset q faz a camera se mover. enqt q o target é em quem(ou o que) focar
        camera_relativa->camera.offset.x += forca_horizontal;
        camera_relativa->camera.offset.y += forca_vertical;


        camera_relativa->duracao -= GetFrameTime();
    }
    else
    {
        //reinicializacoes
        camera_relativa->forca = 0;
        camera_relativa->camera.offset.x = (LARGURA)/2;
        camera_relativa->camera.offset.y = (ALTURA)/2;
        camera_relativa->ativa = false;
    }
}


/**
 * @brief Função que aplica o efeito de zoom in ou zoom out na câmera.
 * @param camera_relativa Ponteiro para a estrutura da câmera.
 * @param zoom_destino Valor de zoom desejado.
 */
void zoomInOut(tCamera* camera_relativa, float zoom_destino)
{
    //calculo da forca
    static bool primeira_vez = true;
    if(primeira_vez) 
    {
        camera_relativa->forca = (zoom_destino - camera_relativa->camera.zoom)/(camera_relativa->duracao*90);
        primeira_vez = false;
    }
    //zoom in
    if(camera_relativa->forca > 0)
    {
        camera_relativa->camera.zoom += camera_relativa->forca;
        if(zoom_destino <= camera_relativa->camera.zoom)
        {
            camera_relativa->camera.zoom = zoom_destino;
            camera_relativa->forca = 0;
            camera_relativa->ativa = false;
            primeira_vez = true;
        }   
    }
    //zoom out
    if(camera_relativa->forca < 0)
    {
        camera_relativa->camera.zoom -= fabs(camera_relativa->forca);
        if(zoom_destino >= camera_relativa->camera.zoom)
        {
            camera_relativa->camera.zoom = zoom_destino;
            camera_relativa->forca = 0;
            camera_relativa->ativa = false;
            primeira_vez = true;
        }
        
    }
}