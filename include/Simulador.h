#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Escalonador.h"
#include "Armazem.h"
#include "Auxiliares.h"
#include "Transporte.h"
#include <iostream>
#include <iomanip>
using namespace std;

class Simulador {
private:
    Escalonador* escalonador;
    Armazem** armazens;
    Grafo* grafo;
    int numArmazens;
    int tempoAtual;
    int primeiroTransporte;
    bool primeiroTransporteAgendado;
    void processarChegada(Evento* evento);
    void processarTransporte(Evento* evento);
    int capacidadeTransporte;
    int latenciaTransporte;
    int intervaloTransportes;
    int custoRemocao;
    int ultimoTransporte;

    
public:
    Simulador(int numArmazens, Grafo* grafo, int capacidadeEscalonador);
    ~Simulador();
    void setArmazens(Armazem** a);
    void setGrafo(Grafo* g);
    void setNumArmazens(int n);
    void adicionarEvento(Evento* evento);
    void executar(int tempoMaximo);
    void imprimirEstadoAtual();
    void setParametrosTransporte(int capacidade, int latencia, int intervalo, int custo);
    bool podeTransportar(int tempoAtual) const;
};

#endif
