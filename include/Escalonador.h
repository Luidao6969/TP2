#ifndef ESCALONADOR
#define ESCALONADOR

#include "Auxiliares.h"

class Escalonador {
private:
    MinHeap* filaEventos;
public:
    Escalonador(int capacidade, Grafo* grafo, int intervalo, int tempoLimite);
    ~Escalonador();

    void inserirEvento(Evento* e);
    Evento* proximoEvento();
    bool vazio();
};

#endif
