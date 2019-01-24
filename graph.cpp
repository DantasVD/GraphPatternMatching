#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <climits>
#include "graph.h"

using namespace std;

Graph::Graph(int N) {
    this->N = N;
    this->adjL = new list<int>[N]; // Cria N listas de adjacências
    this->V = new Vertex[N];
    for (int i = 0; i < N; i++){
        this->V[i].setId(i);
    }

    // Criando matriz de adjacências
    // this->adjM = new int*[N];
    // for (int i = 0; i < N; i++){
    //     this->adjM[i] = new int[N];
    // }
}

void Graph::addEdge(int v1, int v2)
{
    // Adiciona vértice v2 à lista de vértices adjacentes de v1
    this->adjL[v1].push_back(v2);
    // Adiciona vértice v1 à lista de vértices adjacentes de v2
    //this->adjL[v2].push_back(v1);

    // Adicionando a matriz de adjacências
    //this->adjM[v1][v2] = 1;
    //this->adjM[v2][v1] = 1;

    this->M++; // Incrementa o número de arestas
}

int* Graph::bfs_adjM(int start) {

    bool visited[this->N]; // Array para armazenar se o vértice foi visitado ou não
    int* dist = new int[this->N]; // Distância do vértice inicial

    for (int i = 0; i < this->N; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
    }

    visited[start] = true;
    dist[start] = 0;
    queue<int> Queue;
    Queue.push(start);
    while (! Queue.empty()) {
        int u = Queue.front();
        Queue.pop();
        for(int k = 0; k < this->N; k++) {
            if (this->adjM[u][k] != 0 && visited[k] == false) {                
                visited[k] = true;
                dist[k] = dist[u] + 1;
                Queue.push(k);   
            }            
        }
    }

    return dist;
}

// Implementação com lista de adjacências
int* Graph::bfs_adjL(int start) {

    int visited[this->N]; // Array para armazenar se o vértice foi visitado ou não
    int* dist = new int[this->N]; // Distância do vértice inicial

    for (int i = 0; i < this->N; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
    }

    visited[start] = true;
    dist[start] = 0;
    queue<int> Queue;
    Queue.push(start);

    while (! Queue.empty()) {
        int u = Queue.front(); // Pega o primeiro vértice da fila
        Queue.pop();

        list<int>::iterator it;        
        for(it = adjL[u].begin(); it != adjL[u].end(); it++) { // Para cada vértice na lista de adjacências de u
            if(visited[*it] == false) { // Se ele ainda não foi visitado
                visited[*it] = true;
                dist[*it] = dist[u] + 1; 
                Queue.push(*it); // Coloca vértice na fila
            }
        }
    }

    return dist;
}

int** Graph::distMatrix() {

    // Criando matriz de distancias
    int** dist = new int*[this->N];
    for (int i = 0; i <this-> N; i++){
        dist[i] = new int[N];
    }

    for (int i = 0; i < this->N; i++) {
        dist[i] = this->bfs_adjL(i);
    }

    return dist;
}

int Graph::getN() {
    return this->N;
}

int Graph::getM() {
    return this->M;
}

int** Graph::getAdjM() {
    return this->adjM;
}

list<int>* Graph::getAdjL() {
    return this->adjL;
}

Vertex* Graph::getV() {
    return this->V;
}

int Vertex::getId() {
    return this->id;
}

void Vertex::setId(int id) {
    this->id = id;
}

string Vertex::getLabel() {
    return this->label;
}

void Vertex::setLabel(string label) {
    this->label = label;
}

// Verifica se todos os labels a distância k do centro de Q foram achados 
bool isSolution(int k, set<int> S, map<int,int> vertex_label, vector<list<int>> L, int label_order) {
    //cout << "isSolution com k = " << k <<  endl;

    set<int> Slabels; // Labels que estão em S
    for(auto& v : S) {
        Slabels.insert(vertex_label[v]);
    }

    for(auto& label : L[k + label_order]) { // L[k] --> todos os labels a distância k do centro de Q. É aqui que eu acho que tá o problema
        //cout << "Testando label " << label << endl;
        if (Slabels.count(label) == 0) { // Se algum desse labels não foi encontrado, retorna falso
            //cout << "Não tem" << endl;
            return false;
        }
        //cout << "Achou" << endl;
    }
    return true;
}

set<int> Graph::bfs_Q(int start, int r, int delta, map<int,int> vertex_label, vector<list<int>> L, int label_order) {

    //cout << "Fazendo busca em largura de tamanho " << r << " partindo do vertice " << start << endl;

    set<int> S; // Conjunto dos vértices encontrados pela busca
    bool solution;
    vector<bool> visited(this->N, false); // Array para armazenar se o vértice foi visitado ou não
    vector<int> dist(this->N, INT_MAX);  // Distância do vértice inicial

    visited[start] = true;
    dist[start] = 0;
    queue<int> Queue;
    Queue.push(start);
    int level = 0;

    while (!Queue.empty()) {

        int u = Queue.front(); // Pega o primeiro vértice da fila
        
        Queue.pop(); // Tira u da fila

        //cout << "Tirando vertice " << u << " da fila" << endl;

        list<int>::iterator it;        
        for(it = adjL[u].begin(); it != adjL[u].end(); it++) { // Para cada vértice na lista de adjacências de u

            if(visited[*it] == false) { // Se ele ainda não foi visitado
                visited[*it] = true;
                dist[*it] = dist[u] + 1;
                if (level < dist[*it]) {
                    level = dist[*it];
                    //cout << "Entrando no level " << level << endl;
                }
                // A cada nível (delta, 2*delta, 3*delta, ..., k*delta, ..., r*delta) verifica se todos os labels a distância k do centro de Q foram achados 
                if ((level % delta) == 1 && level > delta) { // Se (level % delta) == 1, significa que a busca acabou de sair de um nível múltiplo de delta
                                                             // Por exemplo: estava em 2*delta e entrou em 2*delta + 1. Como a busca até 2*delta já acabou,
                                                             // verificamos se todos os labels a distância 2 do centro de Q foram achados.
                                                             // Também tem que verificar se level > delta, pois no level 1 também teremos (level % delta) == 1

                    solution = isSolution((level - 1) / delta, S, vertex_label, L, label_order); // level - 1 porque a busca está no level k*delta + 1
                                                                                    // Assim, (k*delta + 1 - 1) / delta = k 
                    level++; 

                    if (!solution) {
                        cout << "Nao tem solucao" << endl;
                        S.clear();
                        return S;
                   }
                }

                if (((dist[*it] - 1) / delta) == r) { // Busca acaba aqui, pois queremos ir até r*delta
                    //cout << "Busca acabou" << endl;
                    return S; // A busca acabou, então retorna os vértices achados
                }
                
                S.insert(*it);
                Queue.push(*it); // Coloca vértice na fila
            }
        }
    }
    return S;
}