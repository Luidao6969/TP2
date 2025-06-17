#ifndef ARMAZEM
#define ARMAZEM

#include <iostream>
#include "../include/Pacote.h"

struct StackPacote {
    Pacote** pacotes;
    int topo;
    int capacidade;

    StackPacote();
    ~StackPacote();
    void push(Pacote* p);
    Pacote* pop();
    bool vazia();
};

class Armazem {
private:
    int id;
    StackPacote* secoes;
    int num_secoes;
public:
    Armazem(int id, int num_secoes);
    ~Armazem();
    StackPacote& getStack(int destino);
    void receberPacote(Pacote* p);
    void listarPacotes() const;
};
#endif