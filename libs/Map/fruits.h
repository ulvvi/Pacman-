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
    if(fruit_timer >= 900){
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
            pacman->current_fruit = 0;
            break;
        case 'S':
            pacman->current_fruit = 1;
            break;
        case 'G':
            pacman->current_fruit = 2;
            break;
        case 'B':
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
        static bool primeira_vez = true;
        if(primeira_vez == true && (((int)pacman->pos.x % TAM_GRID) == 0 && ((int)pacman->pos.y % TAM_GRID) == 0))
        {
            pacman->spd = 4;
            primeira_vez = false;
            pacman->comendo.spritesheet.y = 40;
            pacman->comendo.frame_atual = 0;
            pacman->comendo.tempo_frame = 0.100;
        }
        pacman->curFruitTimer += GetFrameTime();
        
        
        if(pacman->curFruitTimer >= 6)
        {
            pacman->cherry = false;
            pacman->spd = 2;
            pacman->comendo.spritesheet.y = 0;
            pacman->comendo.frame_atual = 0;
            pacman->comendo.tempo_frame = 0.060;
            pacman->curFruitTimer = 0;
            primeira_vez = true;
           
        }  
}

void grape(tJogador* pacman, char** grid_mapa){
        if(pacman->curFruitTimer == 0)
        {
            pacman->comendo.spritesheet.y = 120;
            pacman->comendo.frame_atual = 0;
            pacman->comendo.tempo_frame = 0.100;
            
        }
        pacman->curFruitTimer += GetFrameTime();
       
        int grid_i = pacman->pos.y/40;
        int grid_j = pacman->pos.x/40;

        if(grid_i >= TAM_I || grid_j >= TAM_J || grid_i < 0 || grid_j < 0){
            grid_i = TAM_I - 1;
            grid_j = TAM_J - 1;
        }

        if(pacman->curFruitTimer >= 4 && grid_mapa[grid_i][grid_j] != '#')
        {
            pacman->grape = false;
            pacman->comendo.spritesheet.y = 0;
            pacman->curFruitTimer = 0;
            pacman->comendo.tempo_frame = 0.060;
            
        }  
}

void blueberry(tJogador* pacman){
        if(pacman->curFruitTimer == 0)
        {
            pacman->comendo.spritesheet.y = 160;
            pacman->comendo.frame_atual = 0;
            pacman->comendo.tempo_frame = 0.080;
        }
        pacman->curFruitTimer += GetFrameTime();
        
        if(pacman->curFruitTimer >= 4)
        {
            pacman->blueberry = false;
            pacman->comendo.spritesheet.y = 0;
            pacman->curFruitTimer = 0;
            pacman->comendo.tempo_frame = 0.060;
        }
}

void strawberry(tJogador* pacman, char** grid_mapa){
    int centerX = pacman->pos.x/TAM_GRID;
    int centerY = pacman->pos.y/TAM_GRID;

    int startX = centerX - 3;
    int endX   = centerX + 3;
    int startY = centerY - 3;
    int endY   = centerY + 3;

    DrawRectangle(startX*TAM_GRID, startY*TAM_GRID, 280, 280, RED);

    for (int curY = startY; curY <= endY; curY++){
        for (int curX = startX; curX <= endX; curX++){
            if (curY >= 0 && curY < TAM_I && curX >= 0 && curX < TAM_J){
                
                if (grid_mapa[curY][curX] == '.'){
                    grid_mapa[curY][curX] = ' '; 
                }
            }
        }
    }
    
    pacman->strawberry = false;
}
