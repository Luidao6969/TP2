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

public:
    Transporte(int origem, int destino, int capacidade);
    ~Transporte();
    void carregarPacotes(Armazem* armazemOrigem);
    void descarregarPacotes(Armazem* armazemDestino);
};

#endif
