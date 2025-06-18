#include "../include/Escalonador.h"

Escalonador::Escalonador(int capacidade, Grafo* grafo, int intervalo, int tempoLimite) {
    filaEventos = new MinHeap(capacidade);
}

Escalonador::~Escalonador() {
    delete filaEventos;
}

void Escalonador::inserirEvento(Evento* e) {
    filaEventos->inserir(e);
}

Evento* Escalonador::proximoEvento() {
    return filaEventos->extrairMinimo();
}

bool Escalonador::vazio(){
    return filaEventos->vazia();
}

