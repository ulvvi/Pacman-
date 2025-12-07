#include "player.h"
#include "fantasma.h"

//ESSE EH UM .H não utilizado, n deu tempo de fazer e descartamos///

//enum pra poupar minha vida
typedef enum
{
    EXCLAMODIO,
    PACMAN,
    FANTASMA,
    BOMBA,
    CAVEIRA,
    TOTAL
}CARTAS;

typedef struct
{
    int id;
    Texture2D textura;
}tCartaInfo;

typedef struct
{
    tCartaInfo cartas[TOTAL];
    int slot_1;
    int slot_2;
    int slot_3;
}tJackpot;

//as cartas serao escolhidas mt antes de rodar a animacao e tudo mais, na animacao que vai parecer que ta tudo sendo "uau
//tao aleatorio, nada disso foi calculado previamente pelo computador!"
void escolheCartas(int level, tJackpot* jackpot)
{
    //primeiro slot
    jackpot->slot_1 = jackpot->cartas[GetRandomValue(0, TOTAL-1)].id;
    //segundo slot
    jackpot->slot_2 = jackpot->cartas[GetRandomValue(0, TOTAL-1)].id;
    //terceiro slot(com chances facilitadas de ter um jackpot, seja pro bem ou pro mal. 50% de chance de ser ou nao ser, eis a questao)
    bool vitoria = GetRandomValue(0,1);
    if(vitoria)
    {
        jackpot->slot_3 = jackpot->slot_2;
    }
    else
    {
        bool falsa_esperanca = GetRandomValue(0,1);
        if(falsa_esperanca)
        {
            //engana o player de alguma forma, fazendo-o achar que ia ser um jackpot
        }
        //dar um jeito de escolher qualquer coisa menos a carta que daria o jackpot
    }
    
}