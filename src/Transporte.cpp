#include "../include/Transporte.h"
#include <iostream>

Transporte::Transporte(int origem, int destino, int capacidade)
    : origem(origem), destino(destino), capacidade(capacidade), count(0) {
    pacotes = new Pacote*[capacidade];
}

Transporte::~Transporte() {
    delete[] pacotes;
}

void Transporte::carregarPacotes(Armazem* armazemOrigem) {
    for (int i = 0; i < capacidade; ++i) {
        Pacote* p = armazemOrigem->getStack(destino).pop();
        if (!p) break;
        pacotes[count++] = p;
        std::cout << "Transportando pacote " << p->getId() << " de " << origem << " para " << destino << "\n";
    }
}

void Transporte::descarregarPacotes(Armazem* armazemDestino) {
    for (int i = 0; i < count; ++i) {
        pacotes[i]->avancarNaRota(); // método para mudar origem interna
        armazemDestino->receberPacote(pacotes[i]);
    }
    count = 0;
}
