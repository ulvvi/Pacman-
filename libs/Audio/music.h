#pragma once
#include "../header.h"


/*
    A ideia geral é ter um vetor com as 4 musicas, que serão trocadas dinamicamente de acordo com o jogo
*/

// para serem passados para o switchMusic

//volume normal
float normalVolume = 1.00f;

//recebe o conjunto de musicas e inicia todas ao mesmo tempo para ser dinamico
void playMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        PlayMusicStream(stems[i]);
    }
    return;
}

//pega o conjunto de musicas e atualiza elas00
void updateMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        UpdateMusicStream(stems[i]);
    }
    return;
}

void pauseAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        PauseMusicStream(stems[i]);
    }
    return;
}

void resumeAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        ResumeMusicStream(stems[i]);
    }
    return;
}

void stopAllMusic(Music stems[]){
    for(int i = 0; i < 3; i++){
        StopMusicStream(stems[i]);
    }
    return;
}

/*
    Função recebe um estado e troca a musica;
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

void initiateAudio(Music stems[], int level){

    InitAudioDevice();

    switch(level){
        case 1:
            stems[0] = LoadMusicStream("audio/Music/pause_1.wav");
            stems[1] = LoadMusicStream("audio/Music/level_1.wav");
            stems[2] = LoadMusicStream("audio/Music/power_1.wav");
            break;
        case 2:
            stems[0] = LoadMusicStream("audio/Music/pause_2.wav");
            stems[1] = LoadMusicStream("audio/Music/level_2.wav");
            stems[2] = LoadMusicStream("audio/Music/power_2.wav");
            break;
    }
    return;
}
