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
//autoexplicativo
void ativaCamera(tCamera* camera_relativa, float tempo, float forca)
{
    camera_relativa->duracao = tempo;
    camera_relativa->forca = forca;
    camera_relativa->ativa = true;
}

//treme a tela de acordo com a forca e o tempo passados pela funcao ativaCamera
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

//calcula sua propria forca de acordo com o zoom_destino e o tempo(que é passado pela ativa_camera). serve tanto para zoom in e zoom out
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