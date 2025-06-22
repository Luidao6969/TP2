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

    if (!podeTransportar(tempoAtual)) {
        // Agenda novo evento para quando o intervalo permitir
        int novoTempo = ultimoTransporte + intervaloTransportes;
        Evento* novoEvento = new Evento(novoTempo, 2, idPacote, origem, destino);
        escalonador->inserirEvento(novoEvento);
        return;
    }

    // Atualiza o último tempo de transporte
    ultimoTransporte = tempoAtual;
    
    // Cria transporte com os parâmetros configurados
    Transporte transporte(origem, destino, capacidadeTransporte,
                         latenciaTransporte, custoRemocao);
    
    // 1. Carrega pacotes (adiciona tempo de desempilhar)
    transporte.carregarPacotes(armazens[origem], tempoAtual);
    
    // 2. Descarrega pacotes (adiciona tempo de transporte e empilhamento)
    transporte.descarregarPacotes(armazens[destino], tempoAtual);

    // 3. Verifica pacotes no destino que precisam ser encaminhados
    StackPacote& stackDestino = armazens[destino]->getStack(destino);
    while (!stackDestino.vazia()) {
        Pacote* pacote = stackDestino.pop();
        if (pacote && pacote->getDestino() != destino) {
            int tempoChegada = tempoAtual + custoRemocao; // Tempo para processar
            Evento* novoEvento = new Evento(tempoChegada, 1, 
                                          pacote->getId(), 
                                          destino, 
                                          pacote->getDestino());
            escalonador->inserirEvento(novoEvento);
        }
        // Se for o destino final, o pacote é mantido no armazém
    }
}

void Simulador::executar(int tempoMaximo) {
    while (!escalonador->vazio() && tempoAtual <= tempoMaximo) {
        Evento* evento = escalonador->proximoEvento();
        tempoAtual = evento->getTempo();
        
        // Processa o evento respeitando os tempos
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

void Simulador::setParametrosTransporte(int capacidade, int latencia, int intervalo, int custo) {
    capacidadeTransporte = capacidade;
    latenciaTransporte = latencia;
    intervaloTransportes = intervalo;
    custoRemocao = custo;
    ultimoTransporte = -intervalo; // Permite primeiro transporte no tempo 0
}

bool Simulador::podeTransportar(int tempoAtual) const {
    return (tempoAtual - ultimoTransporte) >= intervaloTransportes;
}
