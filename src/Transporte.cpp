#include "../include/Transporte.h"
#include <iostream>
using namespace std;

Transporte::Transporte(int origem, int destino, int capacidade)
    : origem(origem), destino(destino), capacidade(capacidade), count(0) {
    pacotes = new Pacote*[capacidade];
}

Transporte::~Transporte() {
    delete[] pacotes;
}

void Transporte::carregarPacotes(Armazem* armazemOrigem) {
    if (!armazemOrigem) {
        cerr << "Armazém de origem inválido!" << endl;
        return;
    }

    StackPacote& stack = armazemOrigem->getStack(destino);
    
    for (int i = 0; i < capacidade; ++i) {
        if (stack.vazia()) break;
        
        Pacote* p = stack.pop();
        if (p) {
            if (count < capacidade) {
                pacotes[count++] = p;
                cout << "Carregado pacote " << p->getId() << endl;
            } else {
                cerr << "Capacidade excedida!" << endl;
                stack.push(p); // Devolve o pacote se não couber
                break;
            }
        }
    }
}

void Transporte::descarregarPacotes(Armazem* armazemDestino) {
    if (!armazemDestino) {
        cerr << "Armazém de destino inválido!" << endl;
        return;
    }

    for (int i = 0; i < count; ++i) {
        Pacote* p = pacotes[i];
        if (p) {
            int proximoSalto = p->getProximoPasso();
            if (proximoSalto >= 0) {
                armazemDestino->getStack(proximoSalto).push(p);
                cout << "Descarregado pacote " << p->getId() << " em " << proximoSalto << endl;
            } else {
                cout << "Pacote " << p->getId() << " chegou ao destino final" << endl;
                delete p; // Ou outro tratamento para pacotes entregues
            }
        }
    }
    count = 0; // Reseta o contador após descarregar
}
