#pragma once
#include "../header.h"


/*
    A ideia geral é ter um vetor com as 4 musicas, que serão trocadas dinamicamente de acordo com o jogo
*/
float normalVolume = 1.00f;

/**
 * @brief Toca todas as músicas do conjunto.
 * @param stems Array de músicas.
 */
void playMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        PlayMusicStream(stems[i]);
    }
    return;
}

/**
 * @brief Atualiza todas as músicas do conjunto.
 * @param stems Array de músicas.
 */
void updateMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        UpdateMusicStream(stems[i]);
    }
    return;
}

/**
 * @brief Pausa todas as músicas do conjunto.
 * @param stems Array de músicas.
 */
void pauseAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        PauseMusicStream(stems[i]);
    }
    return;
}

/**
 * @brief Retoma todas as músicas do conjunto.
 * @param stems Array de músicas.
 */
void resumeAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        ResumeMusicStream(stems[i]);
    }
    return;
}

/**
 * @brief Para todas as músicas do conjunto.
 * @param stems Array de músicas.
 */
void stopAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        StopMusicStream(stems[i]);
    }
    return;
}

/**
 * @brief Troca a música atual com base no estado do jogo.
 * @param state Estado atual do jogo.
 * @param stems Array de músicas.
 */
void switchMusic(GameState state, Music stems[]){
    for(int i = 0; i < 3; i++){
         if(i == state){
             SetMusicVolume(stems[i], normalVolume);
         }
         else{
             SetMusicVolume(stems[i], 0.00f);
         }   
    }
}

/**
 * @brief Inicia o áudio do jogo carregando as músicas e efeitos sonoros.
 * @param stems Array de músicas.
 * @param gameSFX Array de sons do jogo.
 * @param menu Ponteiro para a estrutura do menu.
 * @param level Nível atual do jogo.
 */
void initiateAudio(Music stems[], Sound* gameSFX, tMenu* menu, int level){

    switch(level){
        case 2:
            stems[0] = LoadMusicStream("audio/Music/pause_1.wav");
            stems[1] = LoadMusicStream("audio/Music/level_1.wav");
            stems[2] = LoadMusicStream("audio/Music/power_1.wav");
            break;
        case 1:
            stems[0] = LoadMusicStream("audio/Music/pause_2.wav");
            stems[1] = LoadMusicStream("audio/Music/level_2.wav");
            stems[2] = LoadMusicStream("audio/Music/power_2.wav");
            break;
        case 3:
            stems[0] = LoadMusicStream("audio/Music/pause_3.wav");
            stems[1] = LoadMusicStream("audio/Music/level_3.wav");
            stems[2] = LoadMusicStream("audio/Music/power_3.wav");
            break;
        default:
            stems[0] = LoadMusicStream("audio/Music/pause_1.wav");
            stems[1] = LoadMusicStream("audio/Music/level_1.wav");
            stems[2] = LoadMusicStream("audio/Music/power_1.wav");
            break;
    }

    menu->menuSFX[0] = LoadSound("audio/menuSFX/menu1.wav");
    menu->menuSFX[1] = LoadSound("audio/menuSFX/confirm.wav");

    gameSFX[0] = LoadSound("audio/Music/title_end.wav");
    gameSFX[1] = LoadSound("audio/ambiente/jingle.wav");
    SetSoundVolume(gameSFX[1], 2.0f);
    gameSFX[2] = LoadSound("audio/ambiente/win_jingle.wav");
    SetSoundVolume(gameSFX[2], 1.5f);
    gameSFX[3] = LoadSound("audio/ambiente/death.wav");
    SetSoundVolume(gameSFX[3], 2.0f);
    gameSFX[4] = LoadSound("audio/ambiente/ghost_eat.wav");
    SetSoundVolume(gameSFX[4], 1.5f);
    gameSFX[5] = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(gameSFX[5], 0.5f);
    gameSFX[6] = LoadSound("audio/ambiente/use_fruit.wav");
    SetSoundVolume(gameSFX[6], 3.5f);
    gameSFX[7] = LoadSound("audio/ambiente/get_fruit.wav");
    SetSoundVolume(gameSFX[7], 3.0f);

    return;
}
