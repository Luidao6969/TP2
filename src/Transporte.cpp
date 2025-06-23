#include "../include/Transporte.h"
#include <iostream>
using namespace std;

Transporte::Transporte(int origem, int destino, int capacidade, 
                      int tempoTransporte, int tempoDesempilhar)
    : origem(origem), destino(destino), capacidade(capacidade),
      tempoTransporte(tempoTransporte), tempoDesempilhar(tempoDesempilhar),
      count(0) {
    pacotes = new Pacote*[capacidade];
}

Transporte::~Transporte() {
    delete[] pacotes;
}

void Transporte::carregarPacotes(Armazem* armazemOrigem, int& /*tempoAtual*/) {
    if (!armazemOrigem) return;
    
    StackPacote& stack = armazemOrigem->getStack(destino);
    
    for (int i = 0; i < capacidade; ++i) {
        if (stack.vazia()) break;

        Pacote* p = stack.pop();
        if (p) {
            pacotes[count++] = p;
        }
    }
}

void Transporte::descarregarPacotes(Armazem* armazemDestino, int& /*tempoAtual*/) {
    if (!armazemDestino) return;

    for (int i = 0; i < count; ++i) {
        Pacote* p = pacotes[i];
        if (p) {
            int proximoSalto = p->getProximoPasso();
            if (proximoSalto >= 0) {
                armazemDestino->getStack(proximoSalto).push(p);
            }
        }
    }

    count = 0;
}
