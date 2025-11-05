#include "libs/header.h"

//MAIN
int main(void)
{
    int grid_i, grid_j, cronometro = 0;


    //dps mudar pro primeiro state ser o menu
    GameState state_atual = GAMEPLAY;
    Rectangle *colisao_fantasma;
   
    
    /*
    ***********************************
            MAIN WINDOW
    ***********************************
    */
    InitWindow(LARGURA, ALTURA, "PACMAN+"); 
    SetTargetFPS(90);



    /*
    ***********************************
                MENU
    ***********************************
    */
    int option = 0;

    /*
    ***********************************
                AUDIO
    ***********************************
    */
    Music stems[3];
    int level = 1;

    initiateAudio(stems, level);
    playMusic(stems);
    
    Sound som_cut_in = LoadSound("audio/ambiente/CUTIN.mp3");
    SetSoundVolume(som_cut_in, 0.5f);
    
    
    /*
    ***********************************
                TEXTURAS
    ***********************************
    */
    Texture2D cut_in = LoadTexture("sprites/player/pacman_cut_in.png");
    Rectangle spritesheet = {0, 0, 40, 40};
    Texture2D tileset_parede = LoadTexture("sprites/ambiente/tileset_paredes.png");


        
    /*
    ***********************************
                MAPA
    ***********************************
    */
    int** matriz_auxiliar;
    char **grid_mapa = allocateMap();
    matriz_auxiliar = inicializaMatrizAux();
    initMap("maps/mapa1.txt", grid_mapa);
    texturaMapa(grid_mapa, matriz_auxiliar);

    
    /*
    ***********************************
                PLAYER
    ***********************************
    */
    tJogador pacman = {{}, 2, 3, 0, 0, false, 0, 250};
    centralizaPlayer(&pacman, grid_mapa);


    /*
    ***********************************
                JOGO
    ***********************************
    */
    while (!WindowShouldClose())
    {
        //atualiza musicas
        updateMusic(stems);

        //desenhos
        drawBackground(grid_mapa, tileset_parede, spritesheet, pacman, state_atual, matriz_auxiliar);
        DrawFPS(10, 10);

        //RESTANTE DOS LAYERS(NUMA STATE MACHINE)
        switch(state_atual)
        {
            case GAMEPLAY:            
                switchMusic(GAMEPLAY, stems);
                updateLogic(&pacman, grid_mapa, &grid_i, &grid_j, &state_atual, option);

                //ta aqui so de meme depous eu tiro
                if(pacman.power_pellet == true)
                {
                    switchMusic(JACKPOT, stems);
                }

            break;

            case PAUSE:
                switchMusic(MENU, stems);
                menuLogic(&option, &state_atual, grid_mapa);

            break;
            
            //deuixar pa tu refatorar taylor
            case CUT_IN:
                cutIn(som_cut_in, cut_in);
                if(cronometro == 0)
                    PlaySound(som_cut_in);
                    pauseAllMusic(stems);
                    
                if(temporizador(&cronometro) >= 1.5)
                {
                    cronometro = 0;
                    state_atual = GAMEPLAY;
                    resumeAllMusic(stems);
                }
            break;
        }
        EndDrawing();
    }
    //DAR UNLOAD NOS ASSETS
    UnloadTexture(cut_in);
    UnloadSound(som_cut_in);
    UnloadTexture(tileset_parede);

    CloseWindow();
    freeDiddy(grid_mapa);
    freeMatrizAux(matriz_auxiliar);
    return 0;
}







