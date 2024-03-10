// código com objetivo em criar e gerenciar números aleatórios
// utilizando como referência para treino, projeto voltado para criação de jogadas aleatórias para o simon says
randomSeed(analogRead(A0));
Serial.println(random(0,3));

// código para gerar uma nova jogada

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int Mov_Possíveis[100];
    int index;
    int score;
}Jogo;

void Nova_jogada();
void escrever_jogadas();

int main(){
    Jogo Game;
    Game.index = 0;

    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    Nova_jogada(&Game);
    escrever_jogadas(&Game);

    return 0;
}

void Nova_jogada(Jogo* game){
    int nova_opcao = random()%4;
    game->Mov_Possíveis[game->index] = nova_opcao;
    game->index++;
}

void escrever_jogadas(Jogo* game){
    for (int x = 0; x < game->index;x++){
        printf("%d",game->Mov_Possíveis[x]);
    }
}
