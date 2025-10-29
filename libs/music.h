//funçôes da musica

void playMusic(Music* stems){
    for(int i = 0; i < 3; i++){
        PlayMusicStream(stems[i]);
    }
    return;
}

//pega o conjunto de musicas e atualiza elas00
void updateMusic(Music* stems){
    for(int i = 0; i < 3; i++){
        UpdateMusicStream(stems[i]);
    }
    return;
}

void pauseAllMusic(Music* stems){
    for(int i = 0; i < 3; i++){
        PauseMusicStream(stems[i]);
    }
    return;
}

void resumeAllMusic(Music* stems){
    for(int i = 0; i < 3; i++){
        ResumeMusicStream(stems[i]);
    }
    return;
}

void stopAllMusic(Music* stems){
    for(int i = 0; i < 3; i++){
        StopMusicStream(stems[i]);
    }
    return;
}
