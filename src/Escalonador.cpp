#include "../include/Escalonador.h"

Escalonador::Escalonador(int capacidade) {
    fila = new MinHeap(capacidade);
}

Escalonador::~Escalonador() {
    delete fila;
}

void Escalonador::inserirEvento(Evento* e) {
    fila->inserir(e);
}

Evento* Escalonador::proximoEvento() {
    return fila->extrairMinimo();
}

bool Escalonador::vazio() const {
    return fila->vazia();
}