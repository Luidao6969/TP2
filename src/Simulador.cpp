#include "../include/Simulador.h"
#include <iostream>
using namespace std;

Simulador::Simulador(int numArmazens, Grafo* grafo, int capacidadeEscalonador) 
    : numArmazens(numArmazens), grafo(grafo), tempoAtual(0) {
    
    // Inicializa o escalonador com capacidade adequada
    escalonador = new Escalonador(capacidadeEscalonador, grafo, 10, 1000); // Exemplo: intervalo=10, tempoLimite=1000
}

Simulador::~Simulador() {
    // 1. Limpa o escalonador primeiro
    delete escalonador;
    escalonador = nullptr;

    // 2. Limpa os armazéns (se existirem)
    if (armazens) {
        for (int i = 0; i < numArmazens; ++i) {
            if (armazens[i]) {
                // Não deleta os pacotes aqui - já foram tratados
                delete armazens[i];
            }
        }
        delete[] armazens;
        armazens = nullptr;
    }

    // 3. Não deleta o grafo - deve ser deletado pelo main
}

void Simulador::adicionarEvento(Evento* evento) {
    escalonador->inserirEvento(evento);
}

void Simulador::processarChegada(Evento* evento) {
    // Verificação básica
    if (!evento || evento->getOrigem() < 0 || evento->getOrigem() >= numArmazens) {
        cerr << "Erro: Evento inválido ou origem inválida!" << endl;
        return;
    }

    int idPacote = evento->getIdPacote();
    int origem = evento->getOrigem();
    int destino = evento->getDestino();

    // Verifica se o grafo existe
    if (!grafo) {
        cerr << "Erro: Grafo não inicializado!" << endl;
        return;
    }

    // Cria o pacote
    Pacote* pacote = new Pacote(idPacote, origem, destino, tempoAtual, *grafo);
    
    // Verifica se o armazém de origem existe
    if (!armazens[origem]) {
        cerr << "Erro: Armazém de origem " << origem << " não existe!" << endl;
        delete pacote;
        return;
    }

    // Processa normalmente
    armazens[origem]->receberPacote(pacote);

    if (origem != destino) {
        int proximoSalto = pacote->getProximoPasso();
        if (proximoSalto < 0 || proximoSalto >= numArmazens) {
            cerr << "Erro: Próximo salto inválido para pacote " << idPacote << endl;
            return;
        }
        
        int tempoTransporte = tempoAtual + 5;
        Evento* novoEvento = new Evento(tempoTransporte, 2, idPacote, origem, proximoSalto);
        escalonador->inserirEvento(novoEvento);
    }
}

void Simulador::processarTransporte(Evento* evento) {
    int origem = evento->getOrigem();
    int destino = evento->getDestino();
    int idPacote = evento->getIdPacote();

    // Cria transporte e move pacotes
    Transporte transporte(origem, destino, 1); // Capacidade=1
    transporte.carregarPacotes(armazens[origem]);
    transporte.descarregarPacotes(armazens[destino]);

    // Verifica se precisa gerar nova chegada
    Pacote* pacote = armazens[destino]->getStack(destino).pop(); // Simplificação
    if (pacote->getDestino() != destino) {
        int tempoChegada = tempoAtual + 2; // Tempo fixo para exemplo
        Evento* novoEvento = new Evento(tempoChegada, 1, idPacote, destino, pacote->getDestino());
        escalonador->inserirEvento(novoEvento);
    }
}

void Simulador::executar(int tempoMaximo) {
    cout << "Iniciando simulação com " << numArmazens << " armazéns" << endl;
    cout << "Tempo máximo: " << tempoMaximo << endl;

    while (!escalonador->vazio() && tempoAtual <= tempoMaximo) {
        Evento* evento = escalonador->proximoEvento();
        if (!evento) {
            cerr << "Erro: Evento nulo encontrado!" << endl;
            continue;
        }

        tempoAtual = evento->getTempo();
        cout << "\nProcessando evento no tempo " << tempoAtual 
             << " (Tipo: " << evento->getTipo() 
             << ", ID: " << evento->getIdPacote() 
             << ", Origem: " << evento->getOrigem() 
             << ", Destino: " << evento->getDestino() << ")" << endl;

        if (evento->getTipo() == 1) {
            processarChegada(evento);
        } else {
            processarTransporte(evento);
        }

        delete evento;
    }
}

void Simulador::imprimirEstadoAtual(){
    std::cout << "=== Estado no tempo " << tempoAtual << " ===" << std::endl;
    for (int i = 0; i < numArmazens; ++i) {
        std::cout << "Armazém " << i << ": ";
        armazens[i]->listarPacotes();
    }
    std::cout << "=========================" << std::endl;
}

void Simulador::setGrafo(Grafo* g) { 
    grafo = g; 
    if (grafo) {
        cout << "Grafo configurado com " << grafo->getSize() << " nós" << endl;
    }
}
    
void Simulador::setArmazens(Armazem** a) { 
    armazens = a; 
    if (a) {
        cout << "Armazéns configurados" << endl;
    }
}

void Simulador::setNumArmazens(int n) { 
    numArmazens = n; 
    cout << "Número de armazéns configurado: " << n << endl;
}