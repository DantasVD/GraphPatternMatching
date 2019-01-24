#include <iostream>
#include <fstream>
#include <sstream>
#include string>
#include <set>
#include <list>
#include <map>
#include <deque>
#include <chrono>
#include "graph.h"
#include "center.h"
#include "tree.h"

using namespace std;



Graph loadGraphYeast() {
    fstream file;
    file.open("Datasets/YeastS.net");
    string str;
    while(file >> str) {
        if(str == "*vertices"){
            break;
        }
    }
    int N;
    file >> N;
    Graph G(N + 1);
    int id;
    string label;
    while(file >> id >> label) {
        G.getV()[id].setLabel(label);
        if(id == N){
            break;
        }
    }
    file >> str;
    if(str == "*edges"){
        int u, v;
        while(file >> u >> v) {
            if( u != v) {
                G.addEdge(u, v);
                G.addEdge(v, u);
            }
        }
    }
    return G;
}

Graph loadGraphSF() {
    fstream file;
    file.open("Datasets/SF");
    Graph G(100000);
    string line;
    int v;
    int u;
    while (!file.eof()) {
        getline(file, line);
        stringstream stream(line);
        stream >> v; // pega o vértice v
        while (!stream.eof()) { // enquanto ainda tiver vértice na lista de adjacências
            stream >> u; // pega o vértice u
            G.addEdge(v, u);
        }
    }
    return G;
}


void query(Graph& G, Graph& Q, Tree& T, int label, deque<int> order, map<string,int> labelsID, vector<list<int>> label_vertices, int r, int delta, map<int,int> vertex_label, vector<list<int>> L) {
    //cout << "Query para " << v << endl;
    for(int w : label_vertices[label]) { // Para cada vértice de G com esse label faça:
        set<int> S = G.bfs_Q(G.getV()[w].getId(), r, delta, vertex_label, L);
        if(!S.empty()) { // Se S estiver vazio é porque não tem match para esse vértice
            Graph sg = getSubgraph(G, S);
            int next = order.front();
            order.pop_front();
            int next_label = vertex_label[next];
            // Adicionando os vértices na árvore
            for(int u : S) { // Para cada vértice no conjunto S faça:
                if(vertex_label[u] == next_label) { // Adiciona somente os que tem label igual ao do próximo vertice
                    //cout << "Adicionando " << u << " na arvore" << endl;
                    pair<int,int> child (u, next_label);
                    T.addChildToNode(child, w);
                }
            }
            if (order.size() > 1) {
                query(sg, Q, T, next_label, order, labelsID, label_vertices, r-1, delta, vertex_label, L);
            }
        } //else cout << "S vazio" << endl;
    }
}
