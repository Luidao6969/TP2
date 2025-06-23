#include "../include/Simulador.h"

Simulador::Simulador(int numArmazens, Grafo *grafo, int capacidadeEscalonador)
    : numArmazens(numArmazens), grafo(grafo), tempoAtual(0), primeiroTransporte(-1), primeiroTransporteAgendado(false)
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
    // Verificação básica
    if (!evento || evento->getOrigem() < 0 || evento->getOrigem() >= numArmazens)
    {
        cerr << "Erro: Evento inválido ou origem inválida!" << endl;
        return;
    }

    int idPacote = evento->getIdPacote();
    int origem = evento->getOrigem();
    int destino = evento->getDestino();

    // Verifica se o grafo existe
    if (!grafo)
    {
        cerr << "Erro: Grafo não inicializado!" << endl;
        return;
    }

    // Cria o pacote
    Pacote *pacote = new Pacote(idPacote, origem, destino, tempoAtual, *grafo);

    // Verifica se o armazém de origem existe
    if (!armazens[origem])
    {
        cerr << "Erro: Armazém de origem " << origem << " não existe!" << endl;
        delete pacote;
        return;
    }

    // Processa normalmente
    armazens[origem]->receberPacote(pacote);

    if (!primeiroTransporteAgendado)
    {
        primeiroTransporte = tempoAtual + intervaloTransportes;
        escalonador->inserirEvento(new Evento(primeiroTransporte, 2, -1, -1, -1));
        primeiroTransporteAgendado = true;
    }
    // Formata a saída conforme esperado
    cout << setfill('0') << setw(7) << tempoAtual << " pacote "
         << setw(3) << pacote->getId() << " armazenado em "
         << setw(3) << origem << " na secao "
         << setw(3) << pacote->getProximoPasso() << endl;
}

void Simulador::processarTransporte(Evento *evento)
{
    if (!armazens)
        return;

    // Define tempo base deste transporte
    if (tempoAtual < primeiroTransporte)
        tempoAtual = primeiroTransporte;
    if (!podeTransportar(tempoAtual))
        return;

    ultimoTransporte = tempoAtual;

    // Para cada armazém
    for (int origem = 0; origem < numArmazens; ++origem)
    {
        if (!armazens[origem])
            continue;

        // Para cada seção (ou seja, para cada destino possível)
        for (int destino = 0; destino < numArmazens; ++destino)
        {
            if (origem == destino)
                continue;

            StackPacote &stack = armazens[origem]->getStack(destino);
            Pacote **pacotesCarregados = new Pacote *[capacidadeTransporte];
            int count = 0;
            int tempoRemocao = tempoAtual;

            for (int i = 0; i < capacidadeTransporte; ++i)
            {
                if (stack.vazia())
                    break;

                tempoRemocao += custoRemocao;
                Pacote *pacote = stack.pop();
                if (!pacote)
                    continue;

                cout << setfill('0') << setw(7) << tempoRemocao << " pacote "
                     << setw(3) << pacote->getId() << " removido de "
                     << setw(3) << origem << " na secao "
                     << setw(3) << destino << endl;

                cout << setfill('0') << setw(7) << tempoRemocao << " pacote "
                     << setw(3) << pacote->getId() << " em transito de "
                     << setw(3) << origem << " para "
                     << setw(3) << pacote->getProximoPasso() << endl;

                pacotesCarregados[count++] = pacote;
            }

            int tempoChegada = tempoRemocao + latenciaTransporte;

            for (int i = 0; i < count; ++i)
            {
                Pacote *pacote = pacotesCarregados[i];
                if (!pacote)
                    continue;

                pacote->avancarNaRota();
                int atual = pacote->getPosicaoAtual();
                int prox = pacote->getProximoPasso();

                if (atual == pacote->getDestino())
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

            delete[] pacotesCarregados;
        }
    }

    // Agenda próximo transporte global
    int tempoProx = tempoAtual + intervaloTransportes;
    escalonador->inserirEvento(new Evento(tempoProx, 2, -1, -1, -1));
}

void Simulador::executar(int tempoMaximo)
{
    while (!escalonador->vazio() && tempoAtual <= tempoMaximo)
    {
        Evento *evento = escalonador->proximoEvento();
        tempoAtual = evento->getTempo();

        // Processa o evento respeitando os tempos
        if (evento->getTipo() == 1)
        {
            processarChegada(evento);
        }
        else
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
