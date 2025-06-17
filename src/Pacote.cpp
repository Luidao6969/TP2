#include "../include/Pacote.h"
#include <iostream>

using namespace std;

Pacote::Pacote(int id, int origem, int destino, int tempo, Grafo& grafo)
    : id(id), armazem_origem(origem), armazem_destino(destino), tempo_chegada(tempo), estado(0) {
    
    rota = grafo.bfs(origem, destino, tamanho_rota);
    posicao_atual = origem;
}

Pacote::~Pacote() {
    delete[] rota;
}

int Pacote::getId(){
    return id;
}

int Pacote::getOrigem(){
    return armazem_origem;
}

void Pacote::imprimirRota() {
    cout << "Pacote " << id << ": ";
    if (tamanho_rota == 0 || rota == nullptr) {
        cout << "Sem rota.\n";
        return;
    }
    for (int i = 0; i < tamanho_rota; ++i) {
        cout << rota[i];
        if (i != tamanho_rota - 1) cout << " -> ";
    }
    cout << "\n";
}

int Pacote::getDestino(){
    return armazem_destino;
}

int Pacote::getProximoPasso(){
    for (int i = 0; i < tamanho_rota - 1; ++i) {
        if (rota[i] == posicao_atual)
            return rota[i + 1];
    }
    return -1; // já está no destino
}

void Pacote::avancarNaRota() {
    for (int i = 0; i < tamanho_rota - 1; ++i) {
        if (rota[i] == posicao_atual) {
            posicao_atual = rota[i + 1];
            return;
        }
    }
    // já está no destino, não faz nada
}

int Pacote::getTempoChegada(){
    return tempo_chegada;
}