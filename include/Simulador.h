#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "../include/Escalonador.h"
#include "Armazem.h"
#include "../include/Auxiliares.h"

class Simulador {
private:
    Escalonador escalonador;
    Armazem** armazens;
    int numArmazens;
    int capacidadeTransporte;
    int latenciaTransporte;
    int intervaloTransportes;
    int custoRemocao;
    
    void processarEventoPacote(Evento* evento);
    void processarEventoTransporte(Evento* evento);
    
public:
    Simulador(int capacidade, int latencia, int intervalo, int custo, 
              int numArmazens, int** matrizAdj);
    ~Simulador();
    
    void adicionarPacote(Pacote* pacote);
    void executar();
};

#endif