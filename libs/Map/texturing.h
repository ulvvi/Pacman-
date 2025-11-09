#pragma once
#include <math.h>
#include <stdbool.h>
#include "../MainSystem/system.h"

//enumeracoes dos tiles(mais pra debugar msm)
enum 
{   
    FIM_BAIXO,
    FIM_DIREITA,
    CANTO_INF_DIR_F,
    CANTO_INF_DIR_D,
    FIM_ESQUERDA,
    CANTO_INF_ESQ_F,
    CANTO_INF_ESQ_D,
    HORIZONTAL,
    MEIO_ABERTO_CIMA,
    HORIZONTAL_PARA_D_ESQ,
    HORIZONTAL_PARA_D_DIR,
    HORIZONTAL_D,
    FIM_CIMA,
    CIMA,
    CANTO_SUP_DIR_F,
    JUNCAO_T_ESQ,
    VERTICAL_PARA_D_ESQ_CIMA,
    CANTO_SUP_ESQ_F,
    JUNCAO_T_DIR,
    VERTICAL_PARA_D_DIR_CIMA,
    MEIO_ABERTO_BAIXO,
    MEIO_ABERTO_TUDO,
    FAIXA_COMPLETA_MENOS_ESQ_CIMA,
    FAIXA_COMPLETA_MENOS_DIR_CIMA,
    FAIXA_ESQ_BAIXO_DIR_BAIXO,
    CANTO_SUP_DIR_D,
    VERTICAL_PARA_D_ESQ_BAIXO,
    CIMA_BORDA_DIR_D,
    HORIZONTAL_PARA_D_CANTO_ESQ,
    FAIXA_COMPLETA_MENOS_ESQ_BAIXO,
    T_PARA_D_ESQ_HORIZONTAL,
    FAIXA_ESQ_CIMA_DIR_BAIXO,
    JUNCAO_COMUNICANTE_CANTO_SUP_ESQ,
    CANTO_SUP_ESQ_D,
    VERTICAL_PARA_D_DIR_BAIXO,
    CIMA_BORDA_ESQ_D,
    HORIZONTAL_PARA_D_CANTO_DIR,
    FAIXA_COMPLETA_MENOS_DIR_BAIXO,
    FAIXA_DIR_CIMA_ESQ_BAIXO,
    T_PARA_D_DIREITA_HORIZONTAL,
    JUNCAO_COMUNICANTE_CANTO_SUP_DIR,
    HORIZONTAL_BORDA_CIMA_D,
    JUNCAO_CIMA,
    JUNCAO_COMUNICANTE_CANTO_INF_ESQ,
    JUNCAO_COMUNICANTE_CANTO_INF_DIR,
    VAZIO,
    BLOCO_UNICO,
};

//funcao auxiliar, pra ajudar a identificar paredes
bool checaParede(char** matriz, int i, int j)
{
    if(i < 0 || i >= TAM_I || j < 0 || j >= TAM_J)
        return false;
    return matriz[i][j] == '#';
}

//mapeamento dos tiles de acordo com seu bitmask. os indices indicam o bitmask dos tiles
void inicializaMapeamento(int* mapa_mascaras, int tam)
{
    //removendo valores lixo
    for(int i = 0; i < tam; i++)
    {
        mapa_mascaras[i] = -1;
    }
    //mapeamento
    mapa_mascaras[2] = FIM_BAIXO;
    mapa_mascaras[8] = FIM_DIREITA;
    mapa_mascaras[10] = CANTO_INF_DIR_F;
    mapa_mascaras[11] = CANTO_INF_DIR_D;
    mapa_mascaras[16] = FIM_ESQUERDA;
    mapa_mascaras[18] = CANTO_INF_ESQ_F;
    mapa_mascaras[22] = CANTO_INF_ESQ_D;
    mapa_mascaras[24] = HORIZONTAL;
    mapa_mascaras[26] = MEIO_ABERTO_CIMA;
    mapa_mascaras[27] = HORIZONTAL_PARA_D_ESQ;
    mapa_mascaras[30] = HORIZONTAL_PARA_D_DIR;
    mapa_mascaras[31] = HORIZONTAL_D;
    mapa_mascaras[64] = FIM_CIMA;
    mapa_mascaras[66] = CIMA;
    mapa_mascaras[72] = CANTO_SUP_DIR_F;
    mapa_mascaras[74] = JUNCAO_T_ESQ;
    mapa_mascaras[75] = VERTICAL_PARA_D_ESQ_CIMA;
    mapa_mascaras[80] = CANTO_SUP_ESQ_F;
    mapa_mascaras[82] = JUNCAO_T_DIR;
    mapa_mascaras[86] = VERTICAL_PARA_D_DIR_CIMA;
    mapa_mascaras[88] = MEIO_ABERTO_BAIXO;
    mapa_mascaras[90] = MEIO_ABERTO_TUDO;
    mapa_mascaras[91] = FAIXA_COMPLETA_MENOS_ESQ_CIMA;
    mapa_mascaras[94] = FAIXA_COMPLETA_MENOS_DIR_CIMA;
    mapa_mascaras[95] = FAIXA_ESQ_BAIXO_DIR_BAIXO;
    mapa_mascaras[104] = CANTO_SUP_DIR_D;
    mapa_mascaras[106] = VERTICAL_PARA_D_ESQ_BAIXO;
    mapa_mascaras[107] = CIMA_BORDA_DIR_D;
    mapa_mascaras[120] = HORIZONTAL_PARA_D_CANTO_ESQ;
    mapa_mascaras[122] = FAIXA_COMPLETA_MENOS_ESQ_BAIXO;
    mapa_mascaras[123] = T_PARA_D_ESQ_HORIZONTAL;
    mapa_mascaras[126] = FAIXA_ESQ_CIMA_DIR_BAIXO;
    mapa_mascaras[127] = JUNCAO_COMUNICANTE_CANTO_SUP_ESQ;
    mapa_mascaras[208] = CANTO_SUP_ESQ_D;
    mapa_mascaras[210] = VERTICAL_PARA_D_DIR_BAIXO;
    mapa_mascaras[214] = CIMA_BORDA_ESQ_D;
    mapa_mascaras[216] = HORIZONTAL_PARA_D_CANTO_DIR;
    mapa_mascaras[218] = FAIXA_COMPLETA_MENOS_DIR_BAIXO;
    mapa_mascaras[219] = FAIXA_DIR_CIMA_ESQ_BAIXO;
    mapa_mascaras[222] = T_PARA_D_DIREITA_HORIZONTAL;
    mapa_mascaras[223] = JUNCAO_COMUNICANTE_CANTO_SUP_DIR;
    mapa_mascaras[248] = HORIZONTAL_BORDA_CIMA_D;
    mapa_mascaras[250] = JUNCAO_CIMA;
    mapa_mascaras[251] = JUNCAO_COMUNICANTE_CANTO_INF_ESQ;
    mapa_mascaras[254] = JUNCAO_COMUNICANTE_CANTO_INF_DIR;
    mapa_mascaras[255] = VAZIO;
    mapa_mascaras[0] = BLOCO_UNICO;
}
//tabela pra eu n me perder, ignora 
//2 = 1, 8 = 2, 10 = 3, 11 = 4, 16 = 5, 18 = 6, 22 = 7, 
//24 = 8, 26 = 9, 27 = 10, 30 = 11, 31 = 12, 64 = 13, 
//66 = 14, 72 = 15, 74 = 16, 75 = 17, 80 = 18, 82 = 19, 
//86 = 20, 88 = 21, 90 = 22, 91 = 23, 94 = 24, 95 = 25, 
//104 = 26, 106 = 27, 107 = 28, 120 = 29, 122 = 30, 
//123 = 31, 126 = 32, 127 = 33, 208 = 34, 210 = 35, 
//214 = 36, 216 = 37, 218 = 38, 219 = 39, 222 = 40, 
//223 = 41, 248 = 42, 250 = 43, 251 = 44, 254 = 45, 
//255 = 46, 0 = 47

//realiza o calculo da mascara de acordo com o tile central. olha pros 8 blocos adjacentes ao centro pra realizar esse calculo
int calculaMascara(char **matriz, int i, int j)
{   
    //esses vizinhos cardeais é pq tipo, num tile set 8 bit sao 2^8 combinacoes de tile, porem, tem uma repeticao
    //dentro dessas combinacoes todas de tiles, ent de fato so se usa 47. caso um dos cantos do grid 3x3 n tenha vizinhos
    //cardeais, ou seja, nao seja conectado de forma alguma com o tile P central, n tem pq levar ele em consideracao na contagem
    //de pesos, e é isso q evita redundancia e evita tbm ter q mapear 256 tiles com repeticao. com isso, so precisamo de 47(amem)
    bool vizinho_cardeal_no = checaParede(matriz, i-1, j) && checaParede(matriz, i, j-1);
    bool vizinho_cardeal_ne = checaParede(matriz, i-1, j) && checaParede(matriz, i, j+1);
    bool vizinho_cardeal_so = checaParede(matriz, i+1, j) && checaParede(matriz, i, j-1);
    bool vizinho_cardeal_se = checaParede(matriz, i+1, j) && checaParede(matriz, i, j+1);
    //PESOS: 
    //1   2   4
    //8   P   16
    //32  64  128
    
    int somatorio_pesos = 0;

    //noroeste
    if(checaParede(matriz, i-1, j-1) && vizinho_cardeal_no) somatorio_pesos+=1;
    //norte
    if(checaParede(matriz, i-1, j)) somatorio_pesos+=2;
    //nordeste
    if(checaParede(matriz, i-1, j+1) && vizinho_cardeal_ne) somatorio_pesos+=4;
    //oeste
    if(checaParede(matriz, i, j-1)) somatorio_pesos+=8;
    //no centro obviamente nao tem peso, é a parede em si que estamos analisando(é o 'P' ali da tabela de pesos)
    //leste
    if(checaParede(matriz, i, j+1)) somatorio_pesos+=16;
    //sudoeste
    if(checaParede(matriz, i+1, j-1) && vizinho_cardeal_so) somatorio_pesos+=32;
    //sul
    if(checaParede(matriz, i+1, j)) somatorio_pesos+=64;
    //sudeste
    if(checaParede(matriz, i+1, j+1) && vizinho_cardeal_se) somatorio_pesos+=128;
    
    return somatorio_pesos;
}

//alocacao dos tiles no mapa atual em uma matriz(pra n precisar ficar calculando toda hr a cada frame). essa func so deve rodar em inicializacoes ou troca de mapas
void texturizaMapa(int** matriz_auxiliar, int* mapa_mascaras, char** matriz)
{
    for(int i = 0; i < TAM_I; i++)
    {
        for(int j = 0; j < TAM_J; j++)
        {
            if(matriz[i][j] == '#')
            {
                //atribuicao da matriz auxiliar[i][j] a uma respectiva mascara
                matriz_auxiliar[i][j] = mapa_mascaras[calculaMascara(matriz, i, j)];
            }
        }
    }
}

//desenhos dos tiles de acordo com a matriz_auxiliar ja "texturizada"(com as mascaras ja calculadas)
void drawTexturaParede(int** matriz_auxiliar, Texture2D tileset, Rectangle spritesheet)
{
    Vector2 pos;
    for(int i = 0; i < TAM_I; i++) 
    {
        for(int j = 0; j < TAM_J; j++)
        {
            pos.x = j * TAM_GRID;
            pos.y = i * TAM_GRID;
            if(matriz_auxiliar[i][j] != -1)
            {
                //isso da certo pq todos os tiles estao na mesma coluna, vou manter esse padrao   
                spritesheet.x += 40*matriz_auxiliar[i][j];
                DrawTextureRec(tileset, spritesheet, pos, WHITE);
                spritesheet.x = 0;
            }
        }
    }
}

void freeMascaras(int* mapa_mascaras)
{
    free(mapa_mascaras);
}