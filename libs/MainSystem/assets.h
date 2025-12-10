#pragma once
#include "raylib.h"

typedef struct
{
    //player
    Texture2D pacman_sprite;
    Texture2D pacman_comendo_sprite;
    Texture2D pacman_cutscene_morte_sprite;
    Texture2D pacman_vida_sprite;
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
    Texture2D cut_in_animacao;
    Texture2D transicao_animacao;
    Texture2D vitoria_cutscene;
    Texture2D confete_animacao;
    



}tAssets;


void inicializaAssets(tAssets* assets)
{
    //pacman
    assets->pacman_sprite = LoadTexture("sprites/player/pacman_spritesheet.png");
    assets->pacman_comendo_sprite = LoadTexture("sprites/player/animacao_pacman-Sheet.png");
    assets->pacman_cutscene_morte_sprite = LoadTexture("sprites/player/pacman_morte-Sheet.png");
    assets->pacman_vida_sprite = LoadTexture("sprites/ambiente/pacman_hud.png");

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

    //animacoes "soltas"(n ligadas a alguma struct de struct)
    assets->cut_in_animacao = LoadTexture("sprites/player/pacman_cut_in-Sheet.png");
    assets->transicao_animacao = LoadTexture("sprites/ambiente/transicao2-Sheet.png");
    assets->vitoria_cutscene = LoadTexture("sprites/ambiente/vitoria-Sheet.png");
    assets->confete_animacao = LoadTexture("sprites/ambiente/confetes-Sheet.png");
}

void limpezaGeral(tAssets* assets)
{
    //pacman
    UnloadTexture(assets->pacman_sprite);
    UnloadTexture(assets->pacman_comendo_sprite);
    UnloadTexture(assets->pacman_cutscene_morte_sprite);
    UnloadTexture(assets->pacman_vida_sprite);

    //fantasmas
    UnloadTexture(assets->fantasma_normal_sprite);
    UnloadTexture(assets->fantasma_fuga_sprite);
    UnloadTexture(assets->fantasma_morte_sprite);
    UnloadTexture(assets->fantasma_morto_sprite);

    //mapa
    UnloadTexture(assets->mapa_parede_sprite);
    UnloadTexture(assets->cherry);
    UnloadTexture(assets->grape);
    UnloadTexture(assets->blueberry);
    UnloadTexture(assets->strawberry);

    //animacoes "soltas"(n ligadas a alguma struct de struct)
    UnloadTexture(assets->cut_in_animacao);
    UnloadTexture(assets->transicao_animacao);
    UnloadTexture(assets->vitoria_cutscene);
    UnloadTexture(assets->confete_animacao);
}