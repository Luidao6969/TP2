#include "../include/Auxiliares.h"
#include <iostream>

Grafo::Grafo(int size, int** matrix) {
    this->size = size;
    adjMatrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        adjMatrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            adjMatrix[i][j] = matrix[i][j];
        }
    }
}

Grafo::~Grafo() {
    if (adjMatrix) {
        for (int i = 0; i < size; ++i) {
            if (adjMatrix[i]) {
                delete[] adjMatrix[i];
                adjMatrix[i] = nullptr;
            }
        }
        delete[] adjMatrix;
        adjMatrix = nullptr;
    }
}

// Retorna uma cópia segura da matriz de adjacência
int** Grafo::getAdjMatrix(){
        int** copy = new int*[size];
        for (int i = 0; i < size; ++i) {
            copy[i] = new int[size];
            for (int j = 0; j < size; ++j) {
                copy[i][j] = adjMatrix[i][j];
            }
        }
        return copy;
}

    // Método auxiliar para liberar a cópia da matriz
void Grafo::freeMatrix(int** matrix, int size) {
        for (int i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
}

int* Grafo::bfs(int start, int end, int& pathSize) {
    bool* visto = new bool[size];
    int* parent = new int[size];
    int* fila = new int[size];
    int front = 0, rear = 0;

    for (int i = 0; i < size; ++i) {
        visto[i] = false;
        parent[i] = -1;
    }

    visto[start] = true;
    fila[rear++] = start;

    bool found = false;

    while (front < rear) {
        int atual = fila[front++];

        if (atual == end) {
            found = true;
            break;
        }

        for (int i = 0; i < size; ++i) {
            if (adjMatrix[atual][i] && !visto[i]) {
                visto[i] = true;
                parent[i] = atual;
                fila[rear++] = i;
            }
        }
    }

    delete[] fila;

    if (!found) {
        delete[] visto;
        delete[] parent;
        pathSize = 0;
        return nullptr;
    }

    // Reconstrução do caminho
    int tempPathSize = 0;
    for (int v = end; v != -1; v = parent[v]) {
        tempPathSize++;
    }

    int* path = new int[tempPathSize];
    int index = tempPathSize - 1;
    for (int v = end; v != -1; v = parent[v]) {
        path[index--] = v;
    }

    pathSize = tempPathSize;

    delete[] visto;
    delete[] parent;
    return path;
}

bool Grafo::ehVizinho(int u, int v) {
    return adjMatrix[u][v] == 1;
}

int Grafo::getSize(){
    return size;
}

Evento::Evento(int tempo, int tipo, int id_pacote, int origem, int destino)
    : tempo(tempo), tipo(tipo), id_pacote(id_pacote), origem(origem), destino(destino) {}

int Evento::getTempo(){
     return tempo; 
}
int Evento::getTipo(){
    return tipo;
}
int Evento::getIdPacote(){
    return id_pacote; 
}
int Evento::getOrigem(){ 
    return origem; 
}
int Evento::getDestino(){ 
    return destino; 
}

long long Evento::gerarChave(){
    // Ex: tempo = 322, origem = 2, destino = 1, id_pacote = 6, tipo = 1
    // chave = 0003220020011
    long long chave = 0;
    chave += tempo * 10000000LL;
    chave += origem * 100000LL;
    chave += destino * 1000LL;
    chave += id_pacote * 10LL;
    chave += tipo;
    return chave;
}

MinHeap::MinHeap(int capacidade) {
    this->capacidade = capacidade;
    heap = (Evento**)malloc(sizeof(Evento*) * capacidade);
    tamanho = 0;
}

MinHeap::~MinHeap() {
    for (int i = 0; i < tamanho; i++) {
        delete heap[i];
    }
    free(heap);
}

void MinHeap::trocar(int a, int b) {
    Evento* temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

void MinHeap::subir(int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap[i]->gerarChave() < heap[pai]->gerarChave()) {
            trocar(i, pai);
            i = pai;
        } else break;
    }
}

void MinHeap::descer(int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < tamanho && heap[esq]->gerarChave() < heap[menor]->gerarChave())
        menor = esq;
    if (dir < tamanho && heap[dir]->gerarChave() < heap[menor]->gerarChave())
        menor = dir;

    if (menor != i) {
        trocar(i, menor);
        descer(menor);
    }
}

void MinHeap::inserir(Evento* e) {
    if (tamanho >= capacidade) return; // pode expandir aqui se quiser
    heap[tamanho] = e;
    subir(tamanho);
    tamanho++;
}

Evento* MinHeap::extrairMinimo() {
    if (tamanho == 0) return nullptr;
    Evento* min = heap[0];
    heap[0] = heap[--tamanho];
    descer(0);
    return min;
}

bool MinHeap::vazia(){
    return tamanho == 0;
}
