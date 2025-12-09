//inclusões
#pragma once
#include "../MainSystem/system.h"
#include "../header.h"
#include "persegue.h"
#include "intercepta.h"
#include "pincer.h"

enum {
    PERSEGUIDOR,
    INTERCEPTADOR,
    PINCER,
    RANDOM
} ghostTypes;


/**
 ** @brief Função para trocar o sprite do fantasma com base na direção.
 ** @param fantasma Ponteiro para a estrutura do fantasma.
 ** @param numero_fantasma Número total de fantasmas.
 */
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

/**
 ** @brief Função para centralizar todos os fantasmas em suas posições iniciais.
 ** @param fantasma Ponteiro para a estrutura do fantasma.
 ** @param numero_fantasma Número total de fantasmas.
 */
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

/**
 ** @brief Função para verificar se o fantasma está saindo do mapa através dos portais.
 ** @param fantasma Estrutura do fantasma.
 ** @return Direção do portal se estiver saindo, caso contrário retorna -1.
 */
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

/**
 * @brief Função para validar se a direção do fantasma é válida com base no mapa.
 * @param fantasma Estrutura do fantasma.
 * @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 * @param dir Direção a ser validada.
 */
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

/**
 * @brief Função para obter a direção oposta.
 * @param dir Direção atual.
 * @return Direção oposta.
 */
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


/**
 * @brief Função para contar o número de direções válidas para o fantasma.
 * @param fantasma Estrutura do fantasma.
 * @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 * @return Número de direções válidas.
 */
int contaDirecoes(tInimigo fantasma, char** grid_mapa){
    int j=0;
    for(int i=1;i<=4;i++){
        if(validaDirecao(fantasma,grid_mapa,i)!=-1){
            j++;
        }
    }
    return j;
}


/**
 * @brief Função para escolher uma direção aleatória válida para o fantasma.
 * @param fantasma Estrutura do fantasma.
 * @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 * @return Direção escolhida aleatoriamente.
 */
int escolheDirecaoRand(tInimigo fantasma,char** grid_mapa)
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

/**
 * @brief Função para verificar se o fantasma está interceptando o Pacman.
 * @param dir Direção do fantasma.
 * @param fantasma Estrutura do fantasma.
 * @param pacman Estrutura do Pacman.
 * @return true se estiver interceptando, false caso contrário.
 */
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

/**
 * @brief Função para fazer o fantasma fugir do Pacman.
 * @param fantasma Estrutura do fantasma.
 * @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 * @param pacman Estrutura do Pacman.
 * @return Direção escolhida para fugir do Pacman.
 */
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


/**
 * @brief Função para mover o fantasma com base em sua lógica de movimento.
 * @param fantasma Estrutura do fantasma.
 * @param blinky Estrutura do fantasma Blinky (usado para Pincer).
 * @param mapa Estrutura do mapa do jogo.
 * @param indice Índice do ciclo de atualização (usado para temporização).
 * @param pacman Estrutura do Pacman.
 * @return Estrutura do fantasma atualizada após o movimento.
 */
tInimigo moveFantasma(tInimigo fantasma, tInimigo blinky, tMapa mapa, int indice, tJogador pacman){
    
    if(saindoMapa(fantasma)!=-1){
        fantasma.pos=teleportaFantasma(fantasma);
    }
    if((indice%20==0 && (fantasma.pos.x>=40 && fantasma.pos.x<=1520) && (fantasma.pos.y>=40 && fantasma.pos.y<=720)) && !pacman.power_pellet){
        switch(fantasma.type){

            case PERSEGUIDOR:
                fantasma.direcao = escolheDirPersegue(&fantasma, &pacman, &mapa);
                if(fantasma.direcao == -1){
                    fantasma.direcao=escolheDirecaoRand(fantasma, mapa.grid_mapa);
                }
                break;

            case INTERCEPTADOR:
                fantasma.direcao = escolheDirIntercepta(&fantasma, &pacman, &mapa);
                if(fantasma.direcao == -1){
                    fantasma.direcao=escolheDirecaoRand(fantasma, mapa.grid_mapa);
                }
                fantasma.direcao = validaDirecao(fantasma, mapa.grid_mapa, fantasma.direcao);
                break;

            case PINCER:
                fantasma.direcao = escolheDirPinch(&fantasma, &blinky, &pacman, &mapa);
                if(fantasma.direcao == -1){
                    fantasma.direcao=escolheDirecaoRand(fantasma, mapa.grid_mapa);
                }
                fantasma.direcao = validaDirecao(fantasma, mapa.grid_mapa, fantasma.direcao);
                break;

            case RANDOM:
                fantasma.direcao=escolheDirecaoRand(fantasma, mapa.grid_mapa);
                break;
        }
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

/**
 ** @brief Função para calcular o número de fantasmas no mapa.
 ** @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 ** @return Número de fantasmas encontrados no mapa.
 */
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

/**
 ** @brief Função para inicializar os fantasmas com suas propriedades e sprites.
 ** @param fantasma Ponteiro para a estrutura do fantasma.
 ** @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 ** @param assets Estrutura contendo os assets do jogo.
 */
void inicializaFantasmas(tInimigo* fantasma, char** grid_mapa, tAssets assets)
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
                fantasma[contador].type = contador%4;
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
                fantasma[contador].sprite_normal = assets.fantasma_normal_sprite;
                fantasma[contador].sprite_fuga = assets.fantasma_fuga_sprite;
                fantasma[contador].spritesheet.height = 40;
                fantasma[contador].spritesheet.width = 40;
                fantasma[contador].spritesheet.x = 0;
                fantasma[contador].spritesheet.y = 40*(contador%4);

                //animacao morte
                fantasma[contador].morte.frame_atual = 0;
                fantasma[contador].morte.total_frames = 14;
                fantasma[contador].morte.tempo_frame = 0.110;
                fantasma[contador].morte.contador = 0;
                fantasma[contador].morte.sprite = assets.fantasma_morte_sprite;
                fantasma[contador].morte.spritesheet.width = 40;
                fantasma[contador].morte.spritesheet.height = 40;
                fantasma[contador].morte.spritesheet.x = 0;
                fantasma[contador].morte.spritesheet.y = fantasma[contador].spritesheet.y;
                fantasma[contador].morte.rotacao = 0;
                fantasma[contador].morte.escala = 1;

                //animacao morto(descanso)
                fantasma[contador].morto.frame_atual = 0;
                fantasma[contador].morto.total_frames = 7;
                fantasma[contador].morto.tempo_frame = 0.130;
                fantasma[contador].morto.contador = 0;
                fantasma[contador].morto.sprite = assets.fantasma_morto_sprite;
                fantasma[contador].morto.spritesheet.width = 40;
                fantasma[contador].morto.spritesheet.height = 40;
                fantasma[contador].morto.spritesheet.x = 0;
                fantasma[contador].morto.spritesheet.y = fantasma[contador].spritesheet.y;
                fantasma[contador].morto.rotacao = 0;
                fantasma[contador].morto.pos.x = fantasma[contador].pos_inicial.x;
                fantasma[contador].morto.pos.y = fantasma[contador].pos_inicial.y;
                fantasma[contador].morto.escala = 1;
                contador++;
                
            }
        }
    }
}


/**
 * @brief Função para atualizar as caixas de colisão dos fantasmas.
 * @param fantasma Ponteiro para a estrutura do fantasma.
 */
void atualizaColisaoFantasma(tInimigo* fantasma, int n)
{
    for(int i = 0; i < n; i++)
    {
        fantasma[i].colisao_fantasma.x = fantasma[i].pos.x;
        fantasma[i].colisao_fantasma.y = fantasma[i].pos.y;
    }
}


/**
 * @brief Função para checar colisão entre o Pacman e os fantasmas.
 * @param colisao_player Caixa de colisão do Pacman.
 * @param fantasma Ponteiro para a estrutura do fantasma.
 * @param n Número total de fantasmas.
 * @return Índice do fantasma que colidiu, ou -1 se não houver colisão.
 */
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

/**
 * @brief Função para processar o evento de comer um fantasma.
 * @param fantasma Ponteiro para a estrutura do fantasma.
 */
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


/**
 * @brief Função para processar a colisão entre o Pacman e um fantasma.
 * @param pacman Ponteiro para a estrutura do Pacman.
 * @param fantasma Ponteiro para a estrutura do fantasma.
 * @param grid_mapa Mapa do jogo representado como uma matriz de caracteres.
 * @param indice Índice do fantasma que colidiu.
 * @param numero_fantasma Número total de fantasmas.
 * @param state_atual Ponteiro para o estado atual do jogo.
 * @param camera_relativa Ponteiro para a estrutura da câmera relativa.
 * @param pontuacao Ponteiro para a estrutura de efeitos visuais (VFX).
 * @param gameSFX Array de sons do jogo.
 */
void ConcretizaColisao(tJogador* pacman, tInimigo* fantasma, char **grid_mapa, int indice, int numero_fantasma, GameState* state_atual, tCamera* camera_relativa, tVfx* pontuacao, Sound gameSFX[])
{
    if (indice == -1)
        return;
    float tempo_screenshake;
    float forca_screenshake;
    int score = 0;
    switch(pacman->power_pellet)
    {
        case true:
            PlaySound(gameSFX[4]); //som de comer fantasma
            score = 100;
            tempo_screenshake = 0.25;
            forca_screenshake = 200;
            comeFantasma(fantasma, indice);
            ativaCamera(camera_relativa, tempo_screenshake, forca_screenshake);
        break;

        case false:
            if(pacman->fruta_ativa != BLUEBERRY){
                PlaySound(gameSFX[3]); //som de morte
                score = -200;
                 //tirei da minha cabeca mesmo o tempo de shake(e pelo visto ta batendo legal)
                tempo_screenshake = pacman->cutscene_morte.total_frames*pacman->cutscene_morte.tempo_frame - 1.25;
                forca_screenshake = 75;
                ativaCamera(camera_relativa, tempo_screenshake, forca_screenshake);
                pacman->vida--;
                //voltar o pacman pro sprite original
                pacman->spritesheet.x = 0;
                *state_atual = MORTE;
            }
            else{
                score = 100;
                tempo_screenshake = 0.25;
                forca_screenshake = 200;
                comeFantasma(fantasma, indice);
                ativaCamera(camera_relativa, tempo_screenshake, forca_screenshake);

            }
        break;
    }
    if(score != 0)
    {
        pontuacao->ativo = true;
        pontuacao->parametro_especial = score;
        if(pacman->score + score < 0)
        {
            pacman->score = 0;
        }
        else
        {
            pacman->score+= score;
        }
    }
}


/**
 * @brief Função para reviver um fantasma após o tempo de morte.
 * @param fantasma Ponteiro para a estrutura do fantasma.
 * @param indice Índice do fantasma a ser revivido.
 */
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
