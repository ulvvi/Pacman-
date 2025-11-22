#include "libs/header.h"

//MAIN
int main(void)
{   
    initializeWindow();
    InitAudioDevice();
    
    int* nivel_atual;
    *nivel_atual = 1;
    //mainMenu();
    while(!WindowShouldClose())
    {
        //roda a func gamelevel e retorna true ou false(true pra vitoria, false pra derrota)
        if(gameLevel(nivel_atual) == true)
        {
            (*nivel_atual)++;
        }
        else
        {
            //coisa de gameover talvez;
        }
    }

    CloseAudioDevice();
    CloseWindow();
}







