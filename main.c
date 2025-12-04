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
        if(mainMenu() == 1) nivel_atual = 1;
        while(true){
            //roda a func gamelevel e retorna true ou false(true pra vitoria, false pra derrota)
            int state = gameLevel(&nivel_atual, assets);

            //iniciar novo jogo
            if(state == -2)
            {
                nivel_atual = 1;
            }
            //ir ao menu principal
            if(state == -1){
                break;
            }
            //venceu a fase
            if(state == 1)
            {
                (nivel_atual)++;
            }
            //perdeu a fase
            if(state == 0)
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







