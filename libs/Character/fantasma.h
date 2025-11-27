//inclusões
#pragma once
#include "../MainSystem/system.h"
#include "../header.h"
#include "persegue.h"
#include "intercepta.h"


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

//"reseta" o fantasma
void centralizaFantasma(tInimigo* fantasma, int numero_fantasma)
{
    for(int i = 0; i < numero_fantasma; i++)
    {
        fantasma[i].pos.x = fantasma[i].pos_inicial.x;
        fantasma[i].pos.y = fantasma[i].pos_inicial.y;
        fantasma[i].direcao = 0;
        //reset do fantasma, caso esteja morto
        fantasma[i].tempo_morto = 0;
        fantasma[i].desenho = true;
        fantasma[i].spd = 2;
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
            fantasma.pos.y=TAM_GRID*(TAM_I);
            break;
        case 2:
            fantasma.pos.x=-TAM_GRID;
            break;
        case 3:
            fantasma.pos.y=-TAM_GRID;
            break;
        case 4:
            fantasma.pos.x=TAM_GRID*(TAM_J);
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


/*int escolheDirecao(tInimigo fantasma,char** grid_mapa)
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
}*/

bool interseptaPacman(int dir, tInimigo fantasma, tJogador pacman){
    int pacX = pacman.pos.x;
    int pacY = pacman.pos.y;
    int fanX = fantasma.pos.x;
    int fanY = fantasma.pos.y;
    int distanciaSegura = 280; 

    if (abs(pacX - fanX) > distanciaSegura && abs(pacY-fanY) > distanciaSegura) {
        return false; 
    }

    switch (dir)
    {
        case 1: // Cima 
            return (pacY < fanY && pacY > (fanY-distanciaSegura));
        
        case 2: // Direita
            return (pacX > fanX && pacX < (fanX + distanciaSegura));

        case 3: // Baixo
            return (pacY > fanY && pacY < (fanY + distanciaSegura));

        case 4: // Esquerda 
            return (pacX < fanX && pacX > (fanX - distanciaSegura));
    }
    return false;
}

int fogePacman(tInimigo fantasma, char** grid_mapa, tJogador pacman){
    int pacX = pacman.pos.x;
    int pacY = pacman.pos.y;
    int fanX = fantasma.pos.x;
    int fanY = fantasma.pos.y;
    
    int dirAtual = fantasma.direcao;
    int oposta = direcaoOposta(dirAtual);

    int fugirX = 0;
    int fugirY = 0;

    if(pacX < fanX) fugirX = 2; 
    else fugirX = 4;

    if(pacY < fanY) fugirY = 3;
    else fugirY = 1;
    
    if(dirAtual == fugirX || dirAtual == fugirY) {
        if (validaDirecao(fantasma, grid_mapa, dirAtual) != -1) {
            return dirAtual;
        }
    }
    
    int distX = abs(pacX - fanX);
    int distY = abs(pacY - fanY);
    
    int primeiraOpcao, segundaOpcao;
    if(distX < distY) { 
        primeiraOpcao = fugirX;
        segundaOpcao = fugirY;
    } else{
        primeiraOpcao = fugirY;
        segundaOpcao = fugirX;
    }
    if(validaDirecao(fantasma, grid_mapa, primeiraOpcao) != -1 && primeiraOpcao != oposta) {
        return primeiraOpcao;
    }
    if(validaDirecao(fantasma, grid_mapa, segundaOpcao) != -1 && segundaOpcao != oposta) {
        return segundaOpcao;
    }
    for(int i = 1; i <= 4; i++) {
        if (i != oposta && validaDirecao(fantasma, grid_mapa, i) != -1) {
            return i;
        }
    }
    if(validaDirecao(fantasma, grid_mapa, oposta) != -1) {
        return oposta;
    }
    return dirAtual;
    
}

tInimigo moveFantasma(tInimigo fantasma, tMapa mapa, int indice, tJogador pacman){
    
    if(saindoMapa(fantasma)!=-1){
        fantasma.pos=teleportaFantasma(fantasma);
    }
    if((indice%20==0 && (fantasma.pos.x>=40 && fantasma.pos.x<=1520) && (fantasma.pos.y>=40 && fantasma.pos.y<=720)) && !pacman.power_pellet){
        fantasma.direcao=escolheDirPersegue(&fantasma, &pacman, &mapa);
    }
    if((indice%20==0 && (fantasma.pos.x>=40 && fantasma.pos.x<=1520) && (fantasma.pos.y>=40 && fantasma.pos.y<=720)) && pacman.power_pellet){
        fantasma.direcao=fogePacman(fantasma, mapa.grid_mapa, pacman);
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

/*inicializa tudo envolvendo o fantasma*/
void inicializaFantasmas(tInimigo* fantasma, char** grid_mapa)
{   
    int contador = 0;
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(grid_mapa[i][j] == 'F')
            {
                //movimentacao
                fantasma[contador].spd = 2;
                fantasma[contador].vulneravel = false;
                fantasma[contador].direcao = 0;
                //colisao
                fantasma[contador].colisao_fantasma.height = TAM_GRID;
                fantasma[contador].colisao_fantasma.width = TAM_GRID;
                fantasma[contador].colisao_fantasma.x = fantasma[contador].pos.x;
                fantasma[contador].colisao_fantasma.y = fantasma[contador].pos.y;
                fantasma[contador].tempo_morto = 0;
                //posicao
                fantasma[contador].pos.x = j*TAM_GRID;
                fantasma[contador].pos.y = i*TAM_GRID;
                fantasma[contador].pos_inicial.x = fantasma[contador].pos.x;
                fantasma[contador].pos_inicial.y = fantasma[contador].pos.y;

                //sprites base
                fantasma[contador].desenho = true;
                fantasma[contador].sprite_normal = LoadTexture("sprites/inimigo/fantasma_spritesheet.png");
                fantasma[contador].sprite_fuga = LoadTexture("sprites/inimigo/fantasma_fuga-Sheet.png");
                fantasma[contador].spritesheet.height = 40;
                fantasma[contador].spritesheet.width = 40;
                fantasma[contador].spritesheet.x = 0;
                fantasma[contador].spritesheet.y = 40*contador;

                //animacao morte
                fantasma[contador].morte.frame_atual = 0;
                fantasma[contador].morte.total_frames = 14;
                fantasma[contador].morte.tempo_frame = 0.110;
                fantasma[contador].morte.contador = 0;
                fantasma[contador].morte.sprite = LoadTexture("sprites/inimigo/fantasma_morte-Sheet.png");
                fantasma[contador].morte.spritesheet.width = 40;
                fantasma[contador].morte.spritesheet.height = 40;
                fantasma[contador].morte.spritesheet.x = 0;
                fantasma[contador].morte.spritesheet.y = fantasma[contador].spritesheet.y;
                fantasma[contador].morte.rotacao = 0;

                //animacao morto(descanso)
                fantasma[contador].morto.frame_atual = 0;
                fantasma[contador].morto.total_frames = 7;
                fantasma[contador].morto.tempo_frame = 0.130;
                fantasma[contador].morto.contador = 0;
                fantasma[contador].morto.sprite = LoadTexture("sprites/inimigo/fantasma_morto-Sheet.png");
                fantasma[contador].morto.spritesheet.width = 40;
                fantasma[contador].morto.spritesheet.height = 40;
                fantasma[contador].morto.spritesheet.x = 0;
                fantasma[contador].morto.spritesheet.y = fantasma[contador].spritesheet.y;
                fantasma[contador].morto.rotacao = 0;
                fantasma[contador].morto.pos.x = fantasma[contador].pos_inicial.x;
                fantasma[contador].morto.pos.y = fantasma[contador].pos_inicial.y;
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

/*funcao pra o fantasma ser comido*/
void comeFantasma(tInimigo* fantasma, int indice)
{
    fantasma[indice].pos.x = fantasma[indice].pos_inicial.x;
    fantasma[indice].pos.y = fantasma[indice].pos_inicial.y;
    fantasma[indice].colisao_fantasma.height = 0;
    fantasma[indice].colisao_fantasma.width = 0;
    fantasma[indice].spd = 0;
    fantasma[indice].direcao = 0;
    fantasma[indice].tempo_morto = 10;
    fantasma[indice].desenho = false;
    printf("%d", indice);
}

/*SUBTRAI A VIDA DO JOGADOR E, SE NECESSARIO, DA GAMEOVER*/
void ConcretizaColisao(tJogador* pacman, tInimigo* fantasma, char **grid_mapa, int indice, int numero_fantasma, GameState* state_atual, tCamera* camera_relativa)
{
    if (indice == -1)
        return;
    switch(pacman->power_pellet)
    {
        case true:
            float tempo_screenshake = 0.25;
            float forca_screenshake = 200;
            comeFantasma(fantasma, indice);
            ativaCamera(camera_relativa, tempo_screenshake, forca_screenshake);
        break;

        case false:
            if(pacman->vida == 1)
            {
                gameOver();
            }
            else
            {
                //tirei da minha cabeca mesmo o tempo de shake(e pelo visto ta batendo legal)
                float tempo_screenshake = pacman->cutscene_morte.total_frames*pacman->cutscene_morte.tempo_frame - 1.25;
                float forca_screenshake = 75;
                ativaCamera(camera_relativa, tempo_screenshake, forca_screenshake);
                pacman->vida--;
                //voltar o pacman pro sprite original
                pacman->spritesheet.x = 0;
                *state_atual = MORTE;
            }   
        break;
    }
}

/*temporizador e "revive" o fantasma*/
void reviveFantasma(tInimigo* fantasma, int indice)
{
    fantasma[indice].tempo_morto -= GetFrameTime();
    if(fantasma[indice].tempo_morto <= 0)
    {
        fantasma[indice].colisao_fantasma.width = TAM_GRID;
        fantasma[indice].colisao_fantasma.height = TAM_GRID;
        fantasma[indice].desenho = true;
        fantasma[indice].direcao = 0;
        fantasma[indice].spd = 2;
    }
}


