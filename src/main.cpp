#include <iostream>
#include "../include/Auxiliares.h"
#include "../include/Pacote.h"
#include "../include/Armazem.h"
#include "../include/Transporte.h"

int main() {
    const int numArmazens = 5;
    int raw[5][5] = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {0, 1, 0, 0, 1},
        {0, 0, 1, 1, 0}
    };

    // Construindo grafo
    int** matrix = new int*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        matrix[i] = new int[numArmazens];
        for (int j = 0; j < numArmazens; ++j)
            matrix[i][j] = raw[i][j];
    }
    Grafo grafo(numArmazens, matrix);

    // Criando armazéns
    Armazem* armazens[numArmazens];
    for (int i = 0; i < numArmazens; ++i)
        armazens[i] = new Armazem(i, numArmazens);

    // Criando pacotes
    Pacote* pacotes[5];
    pacotes[0] = new Pacote(1, 0, 4, 10, grafo);
    pacotes[1] = new Pacote(2, 1, 3, 12, grafo);
    pacotes[2] = new Pacote(3, 2, 0, 15, grafo);
    pacotes[3] = new Pacote(4, 4, 1, 18, grafo);
    pacotes[4] = new Pacote(5, 0, 3, 20, grafo);
    pacotes[5] = new Pacote(6, 0, 2, 9, grafo);
    pacotes[6] = new Pacote(7, 1, 4, 14, grafo);
    pacotes[7] = new Pacote(8, 3, 0, 16, grafo);
    pacotes[8] = new Pacote(9, 2, 4, 11, grafo);
    pacotes[9] = new Pacote(10, 4, 0, 13, grafo);

    // Inserir pacotes no armazém de origem
    for (int i = 0; i < 10; ++i) {
        int origem = pacotes[i]->getOrigem();
        int prox = pacotes[i]->getProximoPasso();
        armazens[origem]->getStack(prox).push(pacotes[i]);
    }

    // Simular transporte por algumas rodadas
    for (int rodada = 0; rodada < 3; ++rodada) {
        std::cout << "\n--- Rodada " << rodada + 1 << " ---\n";
        for (int i = 0; i < numArmazens; ++i) {
            for (int j = 0; j < numArmazens; ++j) {
                if (grafo.ehVizinho(i, j)) {
                    Transporte t(i, j, 1); // transporte com capacidade 1
                    StackPacote& stack = armazens[i]->getStack(j);
                    for (int k = 0; k < 2 && !stack.vazia(); ++k) {
                        Pacote* p = stack.pop();
                        std::cout << "Transportando pacote " << p->getId() << " de " << i << " para " << j << "\n";
                        p->avancarNaRota(); // atualiza posição atual do pacote
                        if (p->getProximoPasso() != -1)
                            armazens[j]->getStack(p->getProximoPasso()).push(p);
                        else
                            std::cout << "Pacote " << p->getId() << " chegou ao destino final.\n";
                    }
                }
            }
        }
    }

    // Mostrar pacotes restantes em cada armazém
    std::cout << "\n--- Estado Final dos Armazéns ---\n";
    for (int i = 0; i < numArmazens; ++i) {
        //std::cout << "Armazém " << i << ":\n";
        armazens[i]->listarPacotes();
    }

    // Limpeza
    for (int i = 0; i < 10; ++i)
        delete pacotes[i];
    for (int i = 0; i < numArmazens; ++i)
        delete armazens[i];
    for (int i = 0; i < numArmazens; ++i)
        delete[] matrix[i];
    delete[] matrix;

    return 0;
}
