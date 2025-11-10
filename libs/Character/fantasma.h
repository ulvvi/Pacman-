//inclusões
#include "../MainSystem/system.h"
#include "../Character/player.h"
#include "stdbool.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once

typedef struct
{
    //vai ter q criar um array dessa struct se pa na main, dinamicamente
    //vector2 tem os campos x e y
    Vector2 pos;
    float spd;
    bool vulneravel;
    int direcao;
} tInimigo;

//usei essa func so q pro player, n sei se vai ajudar, mas ta ai
bool checaFantasmaDentroMapa(tInimigo fantasma)
{
    return (fantasma.pos.x > 0 && fantasma.pos.x < TAM_GRID*(TAM_J-1) && fantasma.pos.y > 0 && fantasma.pos.y < TAM_GRID*(TAM_I-1));
}


int validaDirecao(tInimigo fantasma, char** grid_mapa, int dir){
        switch(dir)
            {
            case 1: //cima
                if(grid_mapa[((int)fantasma.pos.y/40)-1][(((int)fantasma.pos.x/40))]!='#')
                {
                    return dir;
                }
                else{return -1;}
            case 2://direita
                if(grid_mapa[((int)fantasma.pos.y/40)][(((int)fantasma.pos.x/40))+1]!='#' )
                {
                   return dir;
                }
                else{return -1;}
            case 3://embaixo
                if(grid_mapa[(((int)fantasma.pos.y/40)+1)][(((int)fantasma.pos.x/40))]!='#')
                {  
                   return dir;
                }
                else{return -1;}
            case 4://esquerda
                if(grid_mapa[(((int)fantasma.pos.y/40))][(((int)fantasma.pos.x/40)-1)]!='#')
                {  
                   return dir;
                }
                else{return -1;}

            }
    
}
int direcaoOposta(int dir){
    switch(dir)
            {
            case 1: //cima
                return 3;
            case 2://direita
                return 4;
            case 3://embaixo
                return 1;
            case 4://esquerda
                return 2;
            }
}
int contaDirecoes(tInimigo fantasma, char** grid_mapa){
    int j=0;
    for(int i=1;i<=4;i++){
        if(validaDirecao(fantasma,grid_mapa,i)!=-1){
            j++;
        }
    }
    return j;
}


int escolheDirecao(tInimigo fantasma,char** grid_mapa)
{   
    int dirDispo = contaDirecoes(fantasma,grid_mapa);
    int dirAtual = fantasma.direcao;
    int dir;

        if(fantasma.direcao>=5 || fantasma.direcao==NULL)//primeira vez: Tem que ser a mesma probabilidade para todos
        {
            do{
                dir = GetRandomValue(1,4);
            }while(validaDirecao(fantasma,grid_mapa,dir)==-1);
            return dir;

        }
        else{ //ja tem alguma direção escolhida: Tem que ter mais chance de continuar nela
            
            int a = GetRandomValue(1,100);
            if(dirDispo<2){
                if((validaDirecao(fantasma,grid_mapa,fantasma.direcao)==-1))//se true -> tem que trocar
                {
                    do{
                        dir = GetRandomValue(1,4);

                    }while(validaDirecao(fantasma,grid_mapa,dir)==-1);
                    return dir;
                }
                else{
                    return fantasma.direcao;
                } 
            }
            else{
                if((validaDirecao(fantasma,grid_mapa,fantasma.direcao)==-1 )) //se true -> tem que trocar
                {
                    do{
                        dir = GetRandomValue(1,4);

                    }while(dir==direcaoOposta(fantasma.direcao) || validaDirecao(fantasma,grid_mapa,dir)==-1 || dir == dirAtual);
                    return dir;
                }
                else{
                    if(a>70 && dirDispo>2){
                        do{
                            dir = GetRandomValue(1,4);
                        }while(dir==fantasma.direcao|| dir==direcaoOposta(fantasma.direcao) || validaDirecao(fantasma,grid_mapa,dir)==-1);
                        return dir;
                    }
                    return fantasma.direcao;
                }

            }
           

        }

       

       

}

tInimigo moveFantasma(tInimigo fantasma,char** grid_mapa, int indice){

    if(indice%20==0){
        fantasma.direcao=escolheDirecao(fantasma, grid_mapa);
    }

        switch (fantasma.direcao){
        case 1:
            if((int)fantasma.pos.x%40==0 ){
            fantasma.pos.y-=fantasma.spd;
            }
            break;
        case 2:

        if((int)fantasma.pos.y%40==0 ){

            fantasma.pos.x+=fantasma.spd;}

            break;

       

        case 3:

        if((int)fantasma.pos.x%40==0 ){

            fantasma.pos.y+=fantasma.spd;}

            break;

       

        case 4:

            if((int)fantasma.pos.y%40==0 ){

            fantasma.pos.x-=fantasma.spd;}

            break;

        }
        return fantasma;
        
        
   

}

int calculaFantasmas(char** grid_mapa)
{
    int numero_fantasmas = 0;
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(grid_mapa[i][j] == 'F')
            {
                numero_fantasmas++;
            }
        }
    }
    return numero_fantasmas;
}

void inicializaFantasmas(tInimigo* fantasma, char** grid_mapa)
{   
    int contador = 0;
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(grid_mapa[i][j] == 'F')
            {
                fantasma[contador].pos.x = j*TAM_GRID;
                fantasma[contador].pos.y = i*TAM_GRID;
                fantasma[contador].spd = 2;
                fantasma[contador].vulneravel = false;
                fantasma[contador].direcao = 0;
                contador++;
            }
        }
    }
}


/*CRIACAO DE RETANGULOS DE COLISAO FANTASMA*/
Rectangle* criaColisaoFantasma(int n)
{
    Rectangle* colisao_fantasma = malloc(sizeof(Rectangle)*n);
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].height = TAM_GRID;
        colisao_fantasma[i].width = TAM_GRID;
    }
    //retorna o endereco da struct alocada
    return colisao_fantasma;
}


/*ATUALIZA COLISAO FANTASMA*/
void atualizaColisaoFantasma(tInimigo* fantasma, Rectangle* colisao_fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        colisao_fantasma[i].x = fantasma[i].pos.x;
        colisao_fantasma[i].y = fantasma[i].pos.y;
    }
}

/*CHECA COLISAO ENTRE O PLAYER E O FINAL, RETORNA O INDICE DO FANTASMA QUE FOI COLIDIDO, OU -1 SE NAO HOUVE COLISAO*/
int checaColisaoFantasma(Rectangle colisao_player, Rectangle* colisao_fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        //funcao booleana, retorne true ou false
        if(CheckCollisionRecs(colisao_player, colisao_fantasma[i]))
        {
            return i;
        }
    }
    return -1;
}


/*SUBTRAI A VIDA DO JOGADOR E, SE NECESSARIO, DA GAMEOVER*/
void ConcretizaColisao(tJogador* pacman, tInimigo inimigo, int n, Vector2* pos_inicial, char **grid_mapa, int indice)
{
    switch(pacman->power_pellet)
    {
        case true:
            //logica de comer o fantasma
        break;

        case false:
            if(pacman->vida > 0)
            {
                pacman->vida--;
                centralizaPlayer(pacman, grid_mapa);
            }
            else
            {
                gameOver();
            }
        break;
    }
}

