#ifndef AUXILIARES
#define AUXILIARES

class Grafo {
public:
    Grafo(int size, int** matrix);
    ~Grafo();
    int* bfs(int start, int end, int& pathSize);
    bool ehVizinho(int u, int v);
private:
    int size;
    int** adjMatrix;
};

class Evento {
private:
    int tempo;         // Tempo em que o evento ocorre
    int tipo;          // 1 = Chegada, 2 = Transporte
    int id_pacote;     // ID do pacote
    int origem;        // Armazém origem
    int destino;       // Armazém destino

public:
    Evento(int tempo, int tipo, int id_pacote, int origem, int destino);

    int getTempo();
    int getTipo();
    int getIdPacote();
    int getOrigem();
    int getDestino();

    // Geração de chave de prioridade para comparação
    long long gerarChave();
};

class MinHeap {
private:
    Evento** heap;
    int capacidade;
    int tamanho;

    void subir(int i);
    void descer(int i);
    void trocar(int a, int b);

public:
    MinHeap(int capacidade);
    ~MinHeap();

    void inserir(Evento* e);
    Evento* extrairMinimo();
    bool vazia();
};

#endif