#include "libs/header.h"

//MAIN

int main(void)
{   
    
    //inicializacoes do jogo(perduram pra sempre)
    initializeWindow();
    InitAudioDevice();
    tAssets assets;
    inicializaAssets(&assets);

    
    int nivel_atual;
    nivel_atual = 1;
    while(!WindowShouldClose())
    {   
        mainMenu();
        while(true){
            //roda a func gamelevel e retorna true ou false(true pra vitoria, false pra derrota)
            int state = gameLevel(&nivel_atual, assets);
            
            if(state == -1){
                break;
            }

            if(state == 1)
            {
                (nivel_atual)++;
            }
            else
            {
                //evitar loop infinito se ja foi apertado o ESC(ou o X)
                if(WindowShouldClose())
                    break;
                int opcao = gameOver();
                if(opcao == 1)
                {
                    //dando break nesse loop while ja roda o mainmenu dnv e tudo ok
                    break;
                }
                //o resto meio q nem precisa, se clicar em rejogar, vai rodar o loop interno novamente, da mesma fase, e muita fé
            }
        }
    }
    limpezaGeral(&assets);
    CloseAudioDevice();
    CloseWindow();
}







