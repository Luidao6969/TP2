#include "../include/Armazem.h"

Armazem::Armazem(int id, int num_secoes) : id(id), num_secoes(num_secoes) {
    secoes = new StackPacote[num_secoes];
}

Armazem::~Armazem() {
    delete[] secoes;
}

void Armazem::limparTudo() {
    if (!secoes) return; // Verificação crítica
    
    for (int i = 0; i < num_secoes; ++i) {
        while (!secoes[i].vazia()) {
            Pacote* p = secoes[i].pop();
            delete p; // Libera cada pacote
        }
    }
}

StackPacote& Armazem::getStack(int proximo_destino) {
    return secoes[proximo_destino];
}

void Armazem::receberPacote(Pacote* p) {
    int proximo = p->getProximoPasso();
    if (proximo >= 0 && proximo < num_secoes)
        secoes[proximo].push(p);
}

void Armazem::listarPacotes() const {
    std::cout << "Armazém " << id << ":\n";
    for (int i = 0; i < num_secoes; ++i) {
        if( i == id) continue;
        std::cout << "  Seção para o Armazém " << i << ": " << secoes[i].topo << " pacotes\n";
    }
}

StackPacote::StackPacote() : topo(0), capacidade(10) {
    pacotes = new Pacote*[capacidade];
}

StackPacote::~StackPacote() {
    delete[] pacotes;
}

void StackPacote::push(Pacote* p) {
    if (topo == capacidade) {
        // Realoca com capacidade dobrada
        capacidade *= 2;
        Pacote** novo = new Pacote*[capacidade];
        for (int i = 0; i < topo; ++i) {
            novo[i] = pacotes[i];
        }
        delete[] pacotes;
        pacotes = novo;
    }
    pacotes[topo++] = p;
}

Pacote* StackPacote::pop() {
    if (topo == 0) return nullptr;
    return pacotes[--topo];
}

bool StackPacote::vazia(){
    return pacotes == nullptr || topo <= 0;
}