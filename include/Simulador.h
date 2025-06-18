#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Escalonador.h"
#include "Armazem.h"
#include "Auxiliares.h"
#include "Transporte.h"

class Simulador {
private:
    Escalonador* escalonador;
    Armazem** armazens;
    Grafo* grafo;
    int numArmazens;
    int tempoAtual;
    void processarChegada(Evento* evento);
    void processarTransporte(Evento* evento);

    
public:
    Simulador(int numArmazens, Grafo* grafo, int capacidadeEscalonador);
    ~Simulador();
    void setArmazens(Armazem** a);
    void setGrafo(Grafo* g);
    void setNumArmazens(int n);
    void adicionarEvento(Evento* evento);
    void executar(int tempoMaximo);
    void imprimirEstadoAtual();
};

#endif
