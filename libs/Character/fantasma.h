//inclusões
#pragma once
#include "../MainSystem/system.h"
#include "../header.h"

//forward declaration
void gameOver();


typedef struct
{
    //vai ter q criar um array dessa struct se pa na main, dinamicamente
    //vector2 tem os campos x e y
    Vector2 pos;
    float spd;
    bool vulneravel;
    int direcao;
    Rectangle colisao_fantasma;
    Vector2 pos_inicial;

    Texture2D sprite;
    Rectangle spritesheet;
} tInimigo;

void trocaSpriteFantasma(tInimigo* fantasma, int numero_fantasma)
{ 
    for(int i = 0; i < numero_fantasma; i++)
    {
        switch(fantasma[i].direcao)
        {
            case 1:
                fantasma[i].spritesheet.x = 40;
            break;

            case 2:
                fantasma[i].spritesheet.x = 80;
            break;

            case 3:
                fantasma[i].spritesheet.x = 0;
            break;

            case 4:
                fantasma[i].spritesheet.x = 120;
            break;
        }    
    }
}

void centralizaFantasma(tInimigo* fantasma, int numero_fantasma)
{
    for(int i = 0; i < numero_fantasma; i++)
    {
        fantasma[i].pos.x = fantasma[i].pos_inicial.x;
        fantasma[i].pos.y = fantasma[i].pos_inicial.y;
        fantasma[i].direcao = 0;
    }
}


int saindoMapa(tInimigo fantasma)
{
    if(fantasma.pos.x == TAM_GRID*(TAM_J)&& fantasma.direcao==2)//portal na direita
    {
        return fantasma.direcao;
    }
    else if(fantasma.pos.x ==-40 && fantasma.direcao==4)//portal na esquerda
    {
        return fantasma.direcao;
    }
    else if(fantasma.pos.y ==-40 && fantasma.direcao==1)//portal acima
    {
        return fantasma.direcao;
    }
    else if(fantasma.pos.y == TAM_GRID*(TAM_I) && fantasma.direcao==3)//portal abaixo
    {
        return fantasma.direcao;
    }
    else {return -1;}  
}
Vector2 teleportaFantasma(tInimigo fantasma){

    switch (fantasma.direcao)
    {
        case 1:
            fantasma.pos.y=TAM_GRID*(TAM_I)-fantasma.spd;
            break;
        case 2:
            fantasma.pos.x=fantasma.spd-TAM_GRID;
            break;
        case 3:
            fantasma.pos.y=fantasma.spd-TAM_GRID;
            break;
        case 4:
            fantasma.pos.x=TAM_GRID*(TAM_J)-fantasma.spd;
            break;
    }
    return fantasma.pos;
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

        if(fantasma.direcao<1 )//primeira vez: Tem que ser a mesma probabilidade para todos
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
    if(saindoMapa(fantasma)!=-1){
        fantasma.pos=teleportaFantasma(fantasma);
    }
    
    if(indice%20==0 && (fantasma.pos.x>=40 && fantasma.pos.x<=1520) && (fantasma.pos.y>=40 && fantasma.pos.y<=720)){
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
                fantasma.pos.x+=fantasma.spd;
            }
            break;

        case 3:
            if((int)fantasma.pos.x%40==0 ){
                fantasma.pos.y+=fantasma.spd;
            }
            break;

        case 4:
            if((int)fantasma.pos.y%40==0 ){
            fantasma.pos.x-=fantasma.spd;
            }
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
                fantasma[contador].colisao_fantasma.height = TAM_GRID;
                fantasma[contador].colisao_fantasma.width = TAM_GRID;
                fantasma[contador].colisao_fantasma.x = fantasma[contador].pos.x;
                fantasma[contador].colisao_fantasma.y = fantasma[contador].pos.y;
                fantasma[contador].pos_inicial.x = fantasma[contador].pos.x;
                fantasma[contador].pos_inicial.y = fantasma[contador].pos.y;
                contador++;
            }
        }
    }
}

/*ATUALIZA COLISAO FANTASMA*/
void atualizaColisaoFantasma(tInimigo* fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        fantasma[i].colisao_fantasma.x = fantasma[i].pos.x;
        fantasma[i].colisao_fantasma.y = fantasma[i].pos.y;
    }
}

/*CHECA COLISAO ENTRE O PLAYER E O FINAL, RETORNA O INDICE DO FANTASMA QUE FOI COLIDIDO, OU -1 SE NAO HOUVE COLISAO*/
int checaColisaoFantasma(Rectangle colisao_player, tInimigo* fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        //funcao booleana, retorne true ou false
        if(CheckCollisionRecs(colisao_player, fantasma[i].colisao_fantasma))
        {
            return i;
        }
    }
    return -1;
}


/*SUBTRAI A VIDA DO JOGADOR E, SE NECESSARIO, DA GAMEOVER*/
void ConcretizaColisao(tJogador* pacman, tInimigo* fantasma, char **grid_mapa, int indice, int numero_fantasma, GameState* state_atual)
{
    if (indice == -1)
        return;
    switch(pacman->power_pellet)
    {
        case true:
            //logica de comer o fantasma
        break;

        case false:
            if(pacman->vida == 1)
            {
                gameOver();
            }
            else
            {
                pacman->vida--;
                //voltar o pacman pro sprite original
                pacman->spritesheet.x = 0;
                centralizaPlayer(pacman, grid_mapa);
                centralizaFantasma(fantasma, numero_fantasma);
                *state_atual = PRIMEIRO_MOVIMENTO;
            }   
        break;
    }
}
