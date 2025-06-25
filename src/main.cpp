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

    // Lê parâmetros de transporte
    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao;
    arquivo >> capacidadeTransporte 
            >> latenciaTransporte
            >> intervaloTransportes
            >> custoRemocao;

    // Configura os parâmetros no simulador
    simulador.setParametrosTransporte(capacidadeTransporte, latenciaTransporte, 
                                    intervaloTransportes, custoRemocao);



    // Lê número de armazéns
    arquivo >> numArmazens;

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

    // Cria armazéns
    armazens = new Armazem*[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        armazens[i] = new Armazem(i, numArmazens);
    }

    // Lê número total de pacotes
    int totalPacotes;
    int idSequencial = 0;
    arquivo >> totalPacotes;

    // Lê pacotes
    string linha;
    getline(arquivo, linha); // Consome a quebra de linha
    
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
        
        if (origem < 0 || origem >= numArmazens || destino < 0 || destino >= numArmazens) {
            cerr << "Erro: Origem ou destino inválido na linha: " << linha << endl;
            continue;
        }

        Evento* evento = new Evento(tempo, 1, idSequencial++, origem, destino);
        simulador.adicionarEvento(evento);
    }

    arquivo.close();
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " arquivo.wkl" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];

    Grafo* grafo = nullptr;
    Armazem** armazens = nullptr;
    int numArmazens = 0;

    {
        // Escopo para garantir destruição do simulador antes dos outros objetos
        Simulador simulador(0, nullptr, 10000);
        
        // Carrega dados
        lerArquivoEntrada(nomeArquivo, simulador, grafo, armazens, numArmazens);
        
        // Configura
        simulador.setGrafo(grafo);
        simulador.setArmazens(armazens);
        simulador.setNumArmazens(numArmazens);
        
        // Executa
        simulador.executar(10000);
        
        // Limpa pacotes explicitamente antes de destruir o simulador
        for (int i = 0; i < numArmazens; ++i) {
            armazens[i]->limparTudo();
        }
    } // Simulador é destruído aqui

    // Libera memória restante
    if (grafo) {
        int** matrix = grafo->getAdjMatrix();
        for (int i = 0; i < grafo->getSize(); ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        delete grafo;
    }

    return 0;
}