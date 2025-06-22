#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Auxiliares.h"
#include "../include/Pacote.h"
#include "../include/Armazem.h"
#include "../include/Transporte.h"
#include "../include/Escalonador.h"
#include "../include/Simulador.h"

using namespace std;

void lerArquivoEntrada(const string& nomeArquivo, Simulador& simulador, Grafo*& grafo, Armazem**& armazens, int& numArmazens) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo de entrada!" << endl;
        exit(1);
    }
    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao;
    // Lê parâmetros de transporte
    arquivo >> capacidadeTransporte 
            >> latenciaTransporte
            >> intervaloTransportes
            >> custoRemocao;

    cout << "Parâmetros de transporte:\n"
         << "  Capacidade: " << capacidadeTransporte << "\n"
         << "  Latência: " << latenciaTransporte << "\n"
         << "  Intervalo: " << intervaloTransportes << "\n"
         << "  Custo remoção: " << custoRemocao << endl;

    // Lê número de armazéns
    arquivo >> numArmazens;
    cout << "Número de armazéns: " << numArmazens << endl;

    // Lê matriz de adjacência
    int** matriz = new int*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        matriz[i] = new int[numArmazens];
        for (int j = 0; j < numArmazens; ++j) {
            if (!(arquivo >> matriz[i][j])) {
                cerr << "Erro ao ler matriz de adjacência!" << endl;
                exit(1);
            }
        }
    }

    // Cria grafo
    grafo = new Grafo(numArmazens, matriz);
    cout << "Grafo criado com " << grafo->getSize() << " armazéns" << endl;

    // Cria armazéns
    armazens = new Armazem*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        armazens[i] = new Armazem(i, numArmazens);
    }

    // Lê número total de pacotes
    int totalPacotes;
    arquivo >> totalPacotes;
    cout << "Total de pacotes: " << totalPacotes << endl;

    // Lê pacotes
    string linha;
    getline(arquivo, linha); // Consome a quebra de linha após o número de pacotes
    
    for (int i = 0; i < totalPacotes; ++i) {
        if (!getline(arquivo, linha)) {
            cerr << "Erro: Arquivo termina antes do esperado!" << endl;
            break;
        }

        if (linha.empty()) continue;
        
        istringstream iss(linha);
        int tempo, id, origem, destino;
        string tempStr, lixo;
        
        if (!(iss >> tempo >> lixo >> id >> lixo >> origem >> lixo >> destino)) {
            cerr << "Erro ao ler linha: " << linha << endl;
            continue;
        }

        cout << "Criando evento: tempo=" << tempo << " id=" << id 
             << " origem=" << origem << " destino=" << destino << endl;
        
        // Verifica valores válidos
        if (origem < 0 || origem >= numArmazens || destino < 0 || destino >= numArmazens) {
            cerr << "Erro: Origem ou destino inválido na linha: " << linha << endl;
            continue;
        }

        Evento* evento = new Evento(tempo, 1, id, origem, destino);
        simulador.adicionarEvento(evento);
    }

    arquivo.close();
}

int main() {
    Grafo* grafo = nullptr;
    Armazem** armazens = nullptr;
    int numArmazens = 0;
    
    {
        // Escopo para garantir destruição do simulador antes dos outros objetos
        Simulador simulador(0, nullptr, 1000);
        
        // Carrega dados
        lerArquivoEntrada("ex1.txt", simulador, grafo, armazens, numArmazens);
        
        // Configura
        simulador.setGrafo(grafo);
        simulador.setArmazens(armazens);
        simulador.setNumArmazens(numArmazens);
        
        // Executa
        simulador.executar(1000);
        
        // Limpa pacotes explicitamente antes de destruir o simulador
        for (int i = 0; i < numArmazens; ++i) {
            armazens[i]->limparTudo();
        }
    } // Simulador é destruído aqui

    // Libera memória restante
    if (grafo) {
        int** matrix = grafo->getAdjMatrix();
        Grafo::freeMatrix(matrix, grafo->getSize());
        delete grafo;
    }

    return 0;
}