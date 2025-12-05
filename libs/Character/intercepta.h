#include "persegue.h"
#include "../header.h"
#pragma once

void whereToIntercept(tJogador* player, int* posXintercepta, int* posYintercepta, tMapa* mapa) {

    int currentX = (int)(player->pos.x / TAM_GRID);
    int currentY = (int)(player->pos.y / TAM_GRID);

    int targetX = currentX; 
    int targetY = currentY; 

    int dirX = (player->move_x / player->spd);
    int dirY = (player->move_y / player->spd);

    const int blocksInAdvance = 8;

    for (int i = 1; i <= blocksInAdvance; i++){
        int potentialX = currentX + (i * dirX);
        int potentialY = currentY + (i * dirY);

        if (potentialY < 0 || potentialY >= TAM_I || potentialX < 0 || potentialX >= TAM_J){
            targetX = currentX + ((i - 1) * dirX);
            targetY = currentY + ((i - 1) * dirY);
            break; 
        }

        if (mapa->grid_mapa[potentialY][potentialX] == '#'){
            targetX = currentX + ((i - 1) * dirX);
            targetY = currentY + ((i - 1) * dirY);
            break;
        }
        targetX = potentialX;
        targetY = potentialY;
    }
    
    *posXintercepta = targetX;
    *posYintercepta = targetY;
}

int escolheDirIntercepta(tInimigo* fantasma, tJogador* player, tMapa* mapa) {

    int posXintercepta = 0;
    int posYintercepta = 0;

    whereToIntercept(player, &posXintercepta, &posYintercepta, mapa);
    
    //converte pra matriz
    int startX = (int)(fantasma->pos.x / TAM_GRID);
    int startY = (int)(fantasma->pos.y / TAM_GRID);
    int targetX = posXintercepta;
    int targetY = posYintercepta;

    //se por alguma razão ele ta dentro do pacman
    if (startX == targetX && startY == targetY) return fantasma->direcao;

    //inicializa o grid de nodes
    static tNode nodeGrid[TAM_I][TAM_J]; 

    for (int y = 0; y < TAM_I; y++) {
        for (int x = 0; x < TAM_J; x++){
            nodeGrid[y][x].x = x;
            nodeGrid[y][x].y = y;
            nodeGrid[y][x].g = INT_MAX;
            nodeGrid[y][x].h = 0;
            nodeGrid[y][x].f = INT_MAX;
            nodeGrid[y][x].explored = false; // Closed List
            nodeGrid[y][x].opened = false;   // Open List
            nodeGrid[y][x].parent = NULL;
        }
    }

    //node inicial na open list
    tNode* startNode = &nodeGrid[startY][startX];
    startNode->g = 0;
    startNode->h = Heuristica(startX, startY, targetX, targetY);
    startNode->f = startNode->g + startNode->h;
    startNode->opened = true;

    // (Cima, Direita, Baixo, Esquerda)
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    //o while principal do A*
    while (true){
        tNode* current = NULL;
        int lowestF = INT_MAX;

        for (int y = 0; y < TAM_I; y++){
            for (int x = 0; x < TAM_J; x++){
                if (nodeGrid[y][x].opened && nodeGrid[y][x].f < lowestF){
                    lowestF = nodeGrid[y][x].f;
                    current = &nodeGrid[y][x];
                }
            }
        }

        // cade o caminho?
        if (current == NULL) break; 

        // Se chegamos ao destino (ou muito perto)
        if (current->x == targetX && current->y == targetY) {
            //deu bosta
            break; //volta filhão
        }

        // move da aberta pa fechada
        current->opened = false;
        current->explored = true;


        for (int i = 0; i < 4; i++){
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            //reconhecer o portal como caminho valido podendo inserir valores
            //fora do grid (basicamente cria aresta no grafo)
            if (newX < 0) newX = TAM_J - 1;
            else if (newX >= TAM_J) newX = 0;

            //mesma coisa so que vertical
            if (newY < 0) newY = TAM_I - 1;
            else if (newY >= TAM_I) newY = 0;

            if (!EhValido(newX, newY, mapa)) continue;

            tNode* neighbor = &nodeGrid[newY][newX];

            if (neighbor->explored) continue;

            int tentativeG = current->g + 1; // custo pra mover é sempre 1 no grid mt brabo

            if (!neighbor->opened || tentativeG < neighbor->g) {
                neighbor->parent = current;
                neighbor->g = tentativeG;
                neighbor->h = Heuristica(newX, newY, targetX, targetY);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->opened = true;
            }
        }
    }

    //backtrack pro primeiro passo pra fazer o caminho
    tNode* pathNode = &nodeGrid[targetY][targetX];

    if (pathNode == NULL){
        return -1; 
    }
    
    //se nao tem caminho, retorna isso pra n dar bosta
    if (pathNode->parent == NULL) return -1;

    // retorna ate o fi do no inicial (pra saber qual o primeiro passo)
    while (pathNode->parent != NULL && pathNode->parent != startNode){
        pathNode = pathNode->parent;
    }

    //casos de portal
    if (startX == TAM_J - 1 && pathNode->x == 0) return DIREITA;
    
    if (startX == 0 && pathNode->x == TAM_J - 1) return ESQUERDA;

    // o passo real do fantasma
    if (pathNode->x > startX) return DIREITA;
    if (pathNode->x < startX) return ESQUERDA;
    if (pathNode->y > startY) return BAIXO;
    if (pathNode->y < startY) return CIM;

    return fantasma->direcao; //caso tudo de errado retorna a direcao atual
}