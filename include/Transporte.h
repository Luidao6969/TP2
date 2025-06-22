#ifndef TRANSPORTE
#define TRANSPORTE

#include "../include/Pacote.h"
#include "../include/Armazem.h"
#include <iostream>

class Transporte {
private:
    int origem;
    int destino;
    int capacidade;
    int count;
    Pacote** pacotes;
    int tempoTransporte;  // Tempo para percorrer o trajeto
    int tempoDesempilhar;

public:
    Transporte(int origem, int destino, int capacidade, int tempoTransporte, int tempoDesempilhar);
    ~Transporte();
    void carregarPacotes(Armazem* armazemOrigem, int& tempoAtual);
    void descarregarPacotes(Armazem* armazemDestino, int& tempoAtual);
    bool estaEmTransito();
};

#endif
