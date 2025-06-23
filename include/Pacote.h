#ifndef PACOTE
#define PACOTE

#include "Auxiliares.h"

class Pacote
{
private:
    int id;
    int armazem_origem;
    int armazem_destino;
    int tempo_chegada;
    int* rota;          
    int tamanho_rota;   
    int estado;
    int posicao_atual;
public:
    Pacote(int id, int origem, int destino, int tempo, Grafo& grafo);
    ~Pacote();
    int getId();
    int getOrigem();
    int getTempoChegada();
    void imprimirRota();
    int getDestino();
    int getProximoPasso();
    int getPosicaoAtual();
    void avancarNaRota();
};

#endif