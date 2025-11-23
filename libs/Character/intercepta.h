//coisas do alg astaritmo A* para perseguir o pacman
#pragma once
#include "../header.h"

int escolheDireçãoIntercepta(tInimigo* fantasma, tJogador* pacman, tMapa* mapa) {
    
    //converte pra matriz
    int startX = (int)(fantasma->pos.x / TAM_GRID);
    int startY = (int)(fantasma->pos.y / TAM_GRID);
    int targetX = (int)(pacman->pos.x / TAM_GRID);
    int targetY = (int)(pacman->pos.y / TAM_GRID);

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

    // Lista de movimentos vizinhos (Cima, Direita, Baixo, Esquerda)
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
    
    //se nao tem caminho, retorna isso pra n dar bosta
    if (pathNode->parent == NULL) return fantasma->direcao;

    // retorna até que o pai do nó atual seja o nó de início
    while (pathNode->parent != NULL && pathNode->parent != startNode) {
        pathNode = pathNode->parent;
    }

    // o passo real do fantasma
    if (pathNode->x > startX) return DIREITA;
    if (pathNode->x < startX) return ESQUERDA;
    if (pathNode->y > startY) return BAIXO;
    if (pathNode->y < startY) return CIM;

    return fantasma->direcao;; //caso tudo de errado retorna a direcao atual
}



