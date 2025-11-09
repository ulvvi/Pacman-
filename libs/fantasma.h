//inclusões
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    //vai ter q criar um array dessa struct se pa na main, dinamicamente
    //vector2 tem os campos x e y
    Vector2 pos;
    float spd;
    bool vulneravel;
    int direcao;
} tInimigo;

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

#include <math.h>

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