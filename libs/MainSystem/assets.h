#pragma once
#include "raylib.h"

typedef struct
{
    //player
    Texture2D pacman_sprite;
    Texture2D pacman_comendo_sprite;
    Texture2D pacman_cutscene_morte_sprite;
    //fantasma
    Texture2D fantasma_normal_sprite;
    Texture2D fantasma_fuga_sprite;
    Texture2D fantasma_morte_sprite;
    Texture2D fantasma_morto_sprite;
    //mapa
    Texture2D mapa_parede_sprite;
    Texture2D cherry;
    Texture2D grape;
    Texture2D blueberry;
    Texture2D strawberry;
    //animacoes
    Texture2D teste;
    Texture2D teste2;

}tAssets;


void inicializaAssets(tAssets* assets)
{
    printf("Tamanho na MAIN: %d bytes\n", sizeof(tAssets));
    /*----------TEXTURAS----------*/
    //pacman
    assets->pacman_sprite = LoadTexture("sprites/player/pacman_spritesheet.png");
    assets->pacman_comendo_sprite = LoadTexture("sprites/player/animacao_pacman-Sheet.png");
    assets->pacman_cutscene_morte_sprite = LoadTexture("sprites/player/pacman_morte-Sheet.png");
    //fantasmas
    assets->fantasma_normal_sprite = LoadTexture("sprites/inimigo/fantasma_spritesheet.png");
    assets->fantasma_fuga_sprite = LoadTexture("sprites/inimigo/fantasma_fuga-Sheet.png");
    assets->fantasma_morte_sprite = LoadTexture("sprites/inimigo/fantasma_morte-Sheet.png");
    assets->fantasma_morto_sprite = LoadTexture("sprites/inimigo/fantasma_morto-Sheet.png");
    //mapa
    assets->mapa_parede_sprite = LoadTexture("sprites/ambiente/tileset_paredes.png");
    assets->cherry = LoadTexture("sprites/ambiente/cereja.png");
    assets->grape = LoadTexture("sprites/ambiente/uva.png");
    assets->blueberry = LoadTexture("sprites/ambiente/mirtilo.png");
    assets->strawberry = LoadTexture("sprites/ambiente/morango.png");
    printf("MORANGO CARREGADO");
    //animacoes "soltas(n ligadas a alguma struct de struct)"

    
    
}

void limpezaGeral(tAssets* assets)
{
    UnloadTexture(assets->pacman_sprite);
    UnloadTexture(assets->pacman_comendo_sprite);
    UnloadTexture(assets->pacman_cutscene_morte_sprite);
    UnloadTexture(assets->fantasma_normal_sprite);
    UnloadTexture(assets->fantasma_fuga_sprite);
    UnloadTexture(assets->fantasma_morte_sprite);
    UnloadTexture(assets->fantasma_morto_sprite);
    UnloadTexture(assets->mapa_parede_sprite);
    UnloadTexture(assets->cherry);
    UnloadTexture(assets->grape);
    UnloadTexture(assets->blueberry);
    UnloadTexture(assets->strawberry);
}