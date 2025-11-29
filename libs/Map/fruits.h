#include <stdio.h>
#include "../header.h"
#include "../MainSystem/system.h"

#define TAM_I 20
#define TAM_J 40
#define TAM_GRID 40

typedef enum {
    CHERRY,
    STRAWBERRY,
    GRAPE,
    BLUEBERRY
} FruitType;

void spawnFruit(char** map, tJogador* pacman){
    static int fruit_timer = 0;
    fruit_timer++;

    //verifica se ja passou o tempo para spawnar a fruta
    if(fruit_timer >= 1800){
        fruit_timer = 0;

        int x, y;
        do {
            x = GetRandomValue(1, TAM_J - 2);
            y = GetRandomValue(1, TAM_I - 2);
        } while (map[y][x] != ' ' || map[y][x] == '.');

        if(map[y][x] == '.'){
            pacman->remainingPellets--;
        }

        //escolhe um tipo de fruta aleatoriamente
        FruitType fruit_type = (FruitType)GetRandomValue(0, 4);
        char fruit_char;
        switch(fruit_type){
            case CHERRY:
                fruit_char = 'C';
                break;
            case STRAWBERRY:
                fruit_char = 'S';
                break;
            case GRAPE:
                fruit_char = 'G';
                break;
            case BLUEBERRY:
                fruit_char = 'B';
                break;
        }

        //coloca a fruta no mapa
        map[y][x] = fruit_char;
    }

}

void getFruit(tJogador* pacman, char fruit_char){
    switch(fruit_char){
        case 'C':
            pacman->score += 500;
            pacman->current_fruit = 0;
            break;
        case 'S':
            pacman->score += 500;
            pacman->current_fruit = 1;
            break;
        case 'G':
            pacman->score += 500;
            pacman->current_fruit = 2;
            break;
        case 'B':
            pacman->score += 500;
            pacman->current_fruit = 3;
            break;
    }
}

void useFruit(tJogador* pacman){
    if(pacman->cherry == false && pacman->strawberry == false && pacman->grape == false && pacman->blueberry == false){
        switch(pacman->current_fruit){
            case CHERRY:
                pacman->cherry = true;
                break;

            case STRAWBERRY:
                pacman->strawberry = true;
                break;

            case GRAPE:
                pacman->grape = true;
                break;

            case BLUEBERRY:
                pacman->blueberry = true;
                break;
        }
        pacman->current_fruit = -1;
    }
}


void cherry(tJogador* pacman){
        pacman->curFruitTimer += GetFrameTime();
        pacman->comendo.spritesheet.y = 40;
        pacman->spd = 4;
        if(pacman->curFruitTimer >= 6)
        {
            pacman->cherry = false;
            pacman->spd = 2;
            pacman->comendo.spritesheet.y = 0;
            pacman->curFruitTimer = 0;
        }  
}

void grape(tJogador* pacman){
        pacman->curFruitTimer += GetFrameTime();
        pacman->comendo.spritesheet.y = 120;
        if(pacman->curFruitTimer >= 4)
        {
            pacman->grape = false;
            pacman->comendo.spritesheet.y = 0;
            pacman->curFruitTimer = 0;
        }  
}

void blueberry(tJogador* pacman){
        pacman->curFruitTimer += GetFrameTime();
        pacman->comendo.spritesheet.y = 160;
        if(pacman->curFruitTimer >= 4)
        {
            pacman->blueberry = false;
            pacman->comendo.spritesheet.y = 0;
            pacman->curFruitTimer = 0;
        }  
}