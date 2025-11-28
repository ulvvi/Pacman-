#include "libs/header.h"

//MAIN
int main(void)
{   
    initializeWindow();
    InitAudioDevice();

    
    int* nivel_atual;
    *nivel_atual = 1;
    while(!WindowShouldClose())
    {   
        mainMenu();
        while(true){
            //roda a func gamelevel e retorna true ou false(true pra vitoria, false pra derrota)
            if(gameLevel(nivel_atual) == true)
            {
                (*nivel_atual)++;
            }
            else
            {
                //gameover()  //coisa de gameover talvez;
                break;
            }
        }
    }

    CloseAudioDevice();
    CloseWindow();
}







