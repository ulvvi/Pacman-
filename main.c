#include "libs/header.h"
typedef enum
{
    NG_PAUSE = -2,
    QUIT_MENU,
    DERROTA,
    VITORIA
}STATE_MAIN;
//MAIN

/**
 * @brief Função principal do jogo, onde o loop principal é executado.
 */
int main(void)
{   
    
    //inicializacoes do jogo(perduram pra sempre)
    initializeWindow();
    InitAudioDevice();
    tAssets assets;
    inicializaAssets(&assets);

    
    int nivel_atual;
    nivel_atual = 1;
    int whatToDo;
    while(!WindowShouldClose())
    {   
        int whatToDo = mainMenu(assets);
        if(whatToDo == 1 || whatToDo == 2) nivel_atual = 1;
        while(true){
            //roda a func gamelevel e retorna true ou false(true pra vitoria, false pra derrota)
            STATE_MAIN state = gameLevel(&nivel_atual, assets, whatToDo);
            whatToDo = 1;

            //iniciar novo jogo a partir do menu
            if(state == NG_PAUSE)
            {
                nivel_atual = 1;
            }
            //ir ao menu principal
            if(state == QUIT_MENU){
                break;
            }
            //passar de fase
            if(state == VITORIA)
            {
                (nivel_atual)++;
            }
            //gameover
            if(state == DERROTA)
            {
                //evitar loop infinito se ja foi apertado o ESC(ou o X)
                if(WindowShouldClose())
                    break;
                int opcao = gameOver(assets);
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







