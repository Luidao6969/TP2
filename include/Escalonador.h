#ifndef ESCALONADOR
#define ESCALONADOR

#include "Auxiliares.h"

class Escalonador {
private:
    MinHeap* fila;

public:
    Escalonador(int capacidade);
    ~Escalonador();

    void inserirEvento(Evento* e);
    Evento* proximoEvento();
    bool vazio() const;
};

#endif