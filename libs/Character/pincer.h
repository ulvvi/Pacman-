#include "persegue.h"
#include "../header.h"
#pragma once

//pega a direcao que o pinch deve seguir
int sign(int x) {
    return (x > 0) - (x < 0);
}

void whereToPinch(tJogador* player, tInimigo* blinky, tMapa* mapa, int* targetX, int* targetY) {
    
    int P_x = (player->pos.x / TAM_GRID);
    int P_y = (player->pos.y / TAM_GRID);
    
    int B_x = (blinky->pos.x / TAM_GRID);
    int B_y = (blinky->pos.y / TAM_GRID);

    int dir_x = (player->move_x / player->spd);
    int dir_y = (player->move_y / player->spd);

    const int blocksToPredict = 2; 
    int R_x = P_x;
    int R_y = P_y;

    if (dir_x == 0 && dir_y < 0) { 
        R_x = P_x - 2; R_y = P_y - 2;
    } else { 
        for (int i = 1; i <= blocksToPredict; i++) {
            int potX = P_x + (i * dir_x);
            int potY = P_y + (i * dir_y);

            //sai do mapa
            if (potY < 0 || potY >= TAM_I || potX < 0 || potX >= TAM_J || 
                mapa->grid_mapa[potY][potX] == '#') {
                
                R_x = P_x + ((i - 1) * dir_x);
                R_y = P_y + ((i - 1) * dir_y);
                goto end_reference_calc;
            }
            R_x = potX; R_y = potY;
        }
    }
    end_reference_calc:;

    
    int T_x = (2 * R_x) - B_x;
    int T_y = (2 * R_y) - B_y;


    //clampeia    
    if (T_x < 0) T_x = 0;
    if (T_x >= TAM_J) T_x = TAM_J - 1;
    if (T_y < 0) T_y = 0;
    if (T_y >= TAM_I) T_y = TAM_I - 1;


    while (mapa->grid_mapa[T_y][T_x] == '#') {
        if (T_x == R_x && T_y == R_y) break;

        int dx = R_x - T_x;
        int dy = R_y - T_y;

        // avança em x
        if (dx != 0) T_x += sign(dx);
        // avança em y
        if (dy != 0) T_y += sign(dy);
    }

    //retorno
    *targetX = T_x;
    *targetY = T_y;
}

int escolheDirPinch(tInimigo* fantasma, tInimigo* blinky, tJogador* player, tMapa* mapa) {

    int posXpinch = 0;
    int posYpinch = 0;

    whereToPinch(player, blinky, mapa, &posXpinch, &posYpinch);
    DrawRectangle(posXpinch*40, posYpinch*40, 40, 40, RED); //desenha o alvo pinch pra teste
    
    //converte pra matriz
    int startX = (int)(fantasma->pos.x / TAM_GRID);
    int startY = (int)(fantasma->pos.y / TAM_GRID);
    int targetX = posXpinch;
    int targetY = posYpinch;

    //se por alguma razão ele ta dentro do pacman
    if (startX == targetX && startY == targetY) return fantasma->direcao;

    //inicializa o grid de nodes
    static tNode nodeGrid[TAM_I][TAM_J]; 

    for (int y = 0; y < TAM_I; y++) {
        for (int x = 0; x < TAM_J; x++) {
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
    while (true) {
        tNode* current = NULL;
        int lowestF = INT_MAX;

        for (int y = 0; y < TAM_I; y++) {
            for (int x = 0; x < TAM_J; x++) {
                if (nodeGrid[y][x].opened && nodeGrid[y][x].f < lowestF) {
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


        for (int i = 0; i < 4; i++) {
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

    if (pathNode == NULL) {
        return -1; 
    }
    
    //se nao tem caminho, retorna isso pra n dar bosta
    if (pathNode->parent == NULL) return -1;

    // retorna ate o fi do no inicial (pra saber qual o primeiro passo)
    while (pathNode->parent != NULL && pathNode->parent != startNode) {
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

    return fantasma->direcao;; //caso tudo de errado retorna a direcao atual
}