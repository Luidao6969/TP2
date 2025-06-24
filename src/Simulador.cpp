#include "../include/Simulador.h"
#include <unordered_set>
#include <vector>

Simulador::Simulador(int numArmazens, Grafo *grafo, int capacidadeEscalonador)
    : numArmazens(numArmazens), grafo(grafo), tempoAtual(0), primeiroTransporte(-1), tempoPrimeiroPacote(-1)
{

    // Inicializa o escalonador com capacidade adequada
    escalonador = new Escalonador(capacidadeEscalonador, grafo, 10, 1000); // Exemplo: intervalo=10, tempoLimite=1000
}

Simulador::~Simulador()
{
    // 1. Limpa o escalonador primeiro
    delete escalonador;
    escalonador = nullptr;

    // 2. Limpa os armazéns (se existirem)
    if (armazens)
    {
        for (int i = 0; i < numArmazens; ++i)
        {
            if (armazens[i])
            {
                // Não deleta os pacotes aqui - já foram tratados
                delete armazens[i];
            }
        }
        delete[] armazens;
        armazens = nullptr;
    }

    // 3. Não deleta o grafo - deve ser deletado pelo main
}

void Simulador::adicionarEvento(Evento *evento)
{
    escalonador->inserirEvento(evento);
}

void Simulador::processarChegada(Evento *evento)
{
    if (!evento || evento->getOrigem() < 0 || evento->getOrigem() >= numArmazens || !grafo)
    {
        cerr << "Erro: Evento inválido ou origem inválida!" << endl;
        return;
    }

    int idPacote = evento->getIdPacote();
    int origem = evento->getOrigem();
    int destino = evento->getDestino();

    Pacote *pacote = new Pacote(idPacote, origem, destino, tempoAtual, *grafo);

    if (!armazens[origem])
    {
        cerr << "Erro: Armazém de origem " << origem << " não existe!" << endl;
        delete pacote;
        return;
    }

    armazens[origem]->receberPacote(pacote);

    // Define o tempo do primeiro transporte apenas no primeiro pacote
    if (tempoPrimeiroPacote == -1)
    {
        tempoPrimeiroPacote = tempoAtual + 1;
        primeiroTransporte = tempoPrimeiroPacote + intervaloTransportes;
        escalonador->inserirEvento(new Evento(primeiroTransporte, 2, -1, -1, -1));
    }

    cout << setfill('0') << setw(7) << tempoAtual << " pacote "
         << setw(3) << pacote->getId() << " armazenado em "
         << setw(3) << origem << " na secao "
         << setw(3) << pacote->getProximoPasso() << endl;
}


void Simulador::processarTransporte(Evento *evento)
{
    if (!armazens || !grafo) return;

    tempoAtual = evento->getTempo();
    ultimoTransporte = tempoAtual;

    const int MAX_PACOTES = 10000;
    bool* pacotesTransportados = new bool[MAX_PACOTES];
    for (int i = 0; i < MAX_PACOTES; ++i) pacotesTransportados[i] = false;

    int capacidadeMaxMovidos = numArmazens * numArmazens * capacidadeTransporte;
    int* origens = new int[capacidadeMaxMovidos];
    int* destinos = new int[capacidadeMaxMovidos];
    Pacote** pacotes = new Pacote*[capacidadeMaxMovidos];
    int movidosCount = 0;

    // Coleta dos pacotes a transportar neste ciclo
    for (int origem = 0; origem < numArmazens; ++origem)
    {
        if (!armazens[origem]) continue;

        for (int destino = 0; destino < numArmazens; ++destino)
        {
            if (origem == destino || !grafo->ehVizinho(origem, destino)) continue;

            StackPacote& stack = armazens[origem]->getStack(destino);
            int count = 0;

            while (!stack.vazia() && count < capacidadeTransporte && movidosCount < capacidadeMaxMovidos)
            {
                Pacote* pacote = stack.top();
                if (pacotesTransportados[pacote->getId()]) break;

                pacote = stack.pop();
                if (!pacote) continue;

                pacotesTransportados[pacote->getId()] = true;

                origens[movidosCount] = origem;
                destinos[movidosCount] = destino;
                pacotes[movidosCount] = pacote;
                ++movidosCount;
                ++count;
            }
        }
    }

    // Ordenação por origem e ID do pacote (bubble sort)
    for (int i = 0; i < movidosCount - 1; ++i) {
        for (int j = 0; j < movidosCount - i - 1; ++j) {
            int origemA = origens[j];
            int origemB = origens[j + 1];
            int idA = pacotes[j]->getId();
            int idB = pacotes[j + 1]->getId();

            if (origemA > origemB || (origemA == origemB && idA > idB)) {
                std::swap(origens[j], origens[j + 1]);
                std::swap(destinos[j], destinos[j + 1]);
                std::swap(pacotes[j], pacotes[j + 1]);
            }
        }
    }

    int tempoRemocao = tempoAtual;
    int tempoChegada = tempoAtual + latenciaTransporte;

    // Bloco 1: Impressão de "removido" e "em transito"
    for (int i = 0; i < movidosCount; ++i)
    {
        int origem = origens[i];
        int destino = destinos[i];
        Pacote* pacote = pacotes[i];

        cout << setfill('0') << setw(7) << tempoRemocao << " pacote "
             << setw(3) << pacote->getId() << " removido de "
             << setw(3) << origem << " na secao "
             << setw(3) << destino << endl;

        cout << setfill('0') << setw(7) << tempoRemocao << " pacote "
             << setw(3) << pacote->getId() << " em transito de "
             << setw(3) << origem << " para "
             << setw(3) << pacote->getProximoPasso() << endl;
    }

    // Bloco 2: Processa avanço e imprime "armazenado" ou "entregue"
    for (int i = 0; i < movidosCount; ++i)
    {
        int destino = destinos[i];
        Pacote* pacote = pacotes[i];

        pacote->avancarNaRota();
        int prox = pacote->getProximoPasso();

        if (pacote->getPosicaoAtual() == pacote->getDestino())
        {
            cout << setfill('0') << setw(7) << tempoChegada << " pacote "
                 << setw(3) << pacote->getId() << " entregue em "
                 << setw(3) << pacote->getDestino() << endl;
            delete pacote;
        }
        else
        {
            armazens[destino]->getStack(prox).push(pacote);
            cout << setfill('0') << setw(7) << tempoChegada << " pacote "
                 << setw(3) << pacote->getId() << " armazenado em "
                 << setw(3) << destino << " na secao "
                 << setw(3) << prox << endl;
        }
    }

    // Libera memória
    delete[] pacotesTransportados;
    delete[] origens;
    delete[] destinos;
    delete[] pacotes;

    // Agenda o próximo transporte
    escalonador->inserirEvento(new Evento(tempoAtual + intervaloTransportes, 2, -1, -1, -1));
}

void Simulador::executar(int tempoMaximo)
{
    while (!escalonador->vazio() && tempoAtual <= tempoMaximo)
    {
        Evento *evento = escalonador->proximoEvento();
        tempoAtual = evento->getTempo();

        if (evento->getTipo() == 1)
        {
            processarChegada(evento);
        }
        else if (evento->getTipo() == 2)
        {
            processarTransporte(evento);
        }

        delete evento;
    }
}

void Simulador::imprimirEstadoAtual()
{
    std::cout << "=== Estado no tempo " << tempoAtual << " ===" << std::endl;
    for (int i = 0; i < numArmazens; ++i)
    {
        std::cout << "Armazém " << i << ": ";
        armazens[i]->listarPacotes();
    }
    std::cout << "=========================" << std::endl;
}

void Simulador::setGrafo(Grafo *g)
{
    grafo = g;
    if (grafo)
    {
        cout << "Grafo configurado com " << grafo->getSize() << " nós" << endl;
    }
}

void Simulador::setArmazens(Armazem **a)
{
    armazens = a;
    if (a)
    {
        cout << "Armazéns configurados" << endl;
    }
}

void Simulador::setNumArmazens(int n)
{
    numArmazens = n;
    cout << "Número de armazéns configurado: " << n << endl;
}

void Simulador::setParametrosTransporte(int capacidade, int latencia, int intervalo, int custo)
{
    capacidadeTransporte = capacidade;
    latenciaTransporte = latencia;
    intervaloTransportes = intervalo;
    custoRemocao = custo;
    ultimoTransporte = -intervalo; // Permite primeiro transporte no tempo 0
}

bool Simulador::podeTransportar(int tempoAtual) const
{
    return (tempoAtual - ultimoTransporte) >= intervaloTransportes;
}
