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

Graph loadGraphYeast();
void query(Graph& G, Graph& Q, Tree& T, int v, deque<int> order, map<string,int> labelsID, vector<list<int>> label_vertices, int r, int delta, map<int,int> vertex_label, vector<list<int>> L, int label_order);
Graph getSubgraph(Graph& G, set<int> S);
list<list<int>> getMatches(Tree& T, int radQ);
void addToOrder(deque<int>& order, Graph& Q, int v, int u, map<string,int> labelsID);

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


Graph getSubgraph(Graph& G, set<int> S) {
    Graph sg(G.getN());
    list<int>* adjL = G.getAdjL();
    for(int v : S) {
        for(int u : adjL[v]) {
            if(S.count(u) == 1) {
                sg.addEdge(v, u);
                sg.addEdge(u, v);
            }
        }
    }
    return sg;
}

list<list<int>> getMatches(Tree& T, int radQ) {
    map<int,int> dist = T.dist(T.getRoot());
    set<int> leafs;
    for(auto& kv : dist) {
        if (kv.second == radQ) {
            leafs.insert(kv.first);
        }
    }
    
    list<list<int>> matches;
    for(int v : leafs) {
        list<int> match;
        while (v != T.getRoot()) {
            match.push_back(v); // Adiciona v no match
            v = T.getParents()[v]; // v = pai de v
        }
        match.push_back(T.getRoot());
        matches.push_back(match);
    }
    return matches;
}

void addToQueue(deque<int>& order, Graph& Q, int v, int u, map<string,int> labelsID) {
    list<int> adjL = Q.getAdjL()[v];
    if(adjL.size() > 1) { // Se esse vértice não for o início ou o fim do caminho
        for(auto& w : adjL) {
            if(w != u) {
                order.push_back(labelsID[Q.getV()[w].getLabel()]);
                addToQueue(order, Q, w, v, labelsID);
            }
        }
    }
}


int main(int argc, char const *argv[]) {

    const int delta = 2;
    Graph G = loadGraphYeast();

    set<string> labels;
    map<string,int> labelsID; // Para associar cada label com um int. Assim a gente fica comparando int em vez de string
    int id = 0;
    for(int i = 1; i < G.getN(); i++) {
        if (labelsID.count(G.getV()[i].getLabel()) == 0) {
            labels.insert(G.getV()[i].getLabel());        
            labelsID.insert(pair<string,int>(G.getV()[i].getLabel(), id));
            id++;
        }
    }

    // for (auto& kv : labelsID) {
    //     cout << "O label " << kv.first << " tem id " << kv.second << endl;
    // }
    
    map<int,int> vertex_label; // Para associar cada vértice com o ID do seu label
    for(int i = 1; i < G.getN(); i++) {
        vertex_label.insert(pair<int,int>(G.getV()[i].getId(), labelsID[G.getV()[i].getLabel()]));
    }

    // for (auto& kv : vertex_label) {
    //     cout << "O vertice " << kv.first << " tem label de id " << kv.second << endl;
    // }

    vector<list<int>> label_vertices(labels.size()); // Para associar cada label com a lista de vértices que têm esse label
    for(int i = 1; i < G.getN(); i++) {        
        label_vertices[labelsID[G.getV()[i].getLabel()]].push_back(i);
    }

    // for(int i = 0; i < labels.size(); i++) {
    //     cout << "Vertices com Label " << i << ": ";
    //     for(const int& v : label_vertices[i]) {
    //         cout << v << " ";
    //     }
    //    cout << endl;
    // }

    const int query_size = 5;
    string query_labels[] = {"\"YJR084W\"", "\"YGR013W\"", "\"YDR432W\"", "\"YPL178W\"", "\"YIL061C\"", "\"YMR125C\"", "\"YKL012W\""};
    //Tamanho atual: 7         1203            160         148             146             149           153           691
    cout << "Query size: " << query_size << endl;

    Graph Q(query_size); // Grafo Q com 5 vértices
    for(int i = 0; i < query_size - 1; i++){// Adicionando arestas
        Q.addEdge(i, i+1);
        Q.addEdge(i+1, i);
    }

    for(int i = 0; i < query_size; i++){
        Q.getV()[i].setLabel(query_labels[i]);
    }

    for(int i = 0; i < query_size; i++){
        Q.getV()[i].setId(i);
    }

    auto start = chrono::steady_clock::now();

    set<int> c = getCenter(Q); // Pega o centro de Q
    //cout << "Centro de Q: " << *c.begin() << endl;
    int radQ = getRad(Q); // Calcula o raio de Q
    //cout << "Raio de Q: " << radQ << endl;

    deque<int> verticesOrder1;
    deque<int> verticesOrder2;

    list<int> adjL = Q.getAdjL()[*c.begin()];

    verticesOrder1.push_back(labelsID[Q.getV()[adjL.front()].getLabel()]); // Ordem dos labels à esquerda do centro 
    verticesOrder2.push_back(labelsID[Q.getV()[adjL.back()].getLabel()]); // Ordem dos labels à direita do centro
                                                                          // Exemplo:
                                                                          // Se o grafo for A-B-C-D-E
                                                                          // Temos verticesOrder1 = [B, A] e verticesOrder2 = [D, E]
                                                                          // Mas essas duas linhas só adicionam o B e D.
                                                                          // Pra adicionar os outros labels usamos a função addToOrder 
    
    addToOrder(verticesOrder1, Q, adjL.front(), *c.begin(), labelsID);
    addToOrder(verticesOrder2, Q, adjL.back(), *c.begin(), labelsID);

    /* cout << "Ordem 1" << endl;;
    while (!verticesOrder1.empty()) {
        int h = verticesOrder1.front();
        cout << h << " ";
        verticesOrder1.pop_front();
    }

    cout << endl;
    cout << "Ordem 2" << endl;;
    while (!verticesOrder2.empty()) {
        int h = verticesOrder2.front();
        cout << h << " ";
        verticesOrder2.pop_front();
    }
 */
    // cout << "Busca em Largura em Q" << endl;
    // for (int i = 0; i < Q.getN(); i++) {
    //     cout << "Distancia vertice " << i << " ao centro: " << distC[i] << endl;
    // }
    // cout << endl;

    // vector<vector<<list<int>>> L(Q.getN());
    // L[*c.begin()].reserve(radQ);
    // for(int i = 0; i < Q.getN(); i++) {
    //     L[i].reserve()
    // }
    
    int* distC = Q.bfs_adjL(*c.begin()); // Distância dos vértices até o centro 

    // Cria radQ + 1 listas que vão guardar os labels a distancia k de c em Q
    // Assim, L[1] tem os labels dos vértice com distância 1 do centro, L[2] os com distância 2, ..., L[k] os com distância k
    // Exemplo: Se Q for A-B-C-D-E, temos:
    // L[0] = [C], L[1] = [B, D], L[2] = [A, E]
    vector<list<int>> L(radQ+1); 
    for(int i = 0; i < Q.getN(); i++) {
        L[distC[i]].push_back(labelsID[Q.getV()[i].getLabel()]);
    }
    delete distC;

    // Aqui, L1 é a distância dos vértices da esquerda e L2 é a distância dos vértices da direita
    // Só que não é mais a distância para o centro, é para o primeiro label do verticesOrder1 (L1) e para o primeiro label do verticesOrder2 (L2)
    // Exemplo: Se Q for A-B-C-D-E, temos:
    // L1[0] = [B], L1[1] = [A], L2[0] = [D], L2[1] = [E]
    // Por isso que para mais de 5 vértices dá errado, porque, para 7 vértices, precisaríamos também das distâncias para o segundo label do verticesOrder1 e 
    // verticesOrder2. Para 9, do terceiro e assim por diante... 
    vector<list<int>> L1(radQ+1); L1[0].push_back(labelsID[Q.getV()[*c.begin()].getLabel()]); // Criando as radQ + 1 listas e adicionando o label do centro
    vector<list<int>> L2;
    if (Q.getN() % 2 == 0) {
        L2.resize(radQ); L2[0].push_back(labelsID[Q.getV()[*c.begin()].getLabel()]); // Criando as radQ listas e adicionando o label do centro
    } else {
        L2.resize(radQ+1); L2[0].push_back(labelsID[Q.getV()[*c.begin()].getLabel()]); // Criando as radQ + 1 listas e adicionando o label do centro
    }

    for(int i = 1; i < radQ+1; i++) {
        for(const int& l : L[i]) { // Para cada label a distância i do centro
            bool s1 = false;
            for(const int& h : verticesOrder1) { // Para cada label do verticesOrder1
                if (h == l) { // Não sei se isso tá certo, acho que era pra ser if (h == l) (antes tava if (vertex_label[h] == l)
                    L1[i - 1].push_back(l);
                    s1 = true;
                }
            }
            if (!s1) {
                L2[i - 1].push_back(l);
            }
        }
    }

    // for(int i = 0; i < radQ+1; i++) {
    //     cout << "Distancia " << i << ": ";
    //     for(const int& d : L1[i]) {
    //         cout << d << " "; 
    //     }
    //     cout << endl;
    // }

    list<list<int>> m1;
    list<list<int>> m2;
    list<list<int>> matches;

    // QUERY
    // Aqui começa a fazer a busca pelos vértices que têm o mesmo label do centro
    for(int v : label_vertices[labelsID[Q.getV()[*c.begin()].getLabel()]]) { // Para cada vértice de G com mesmo label de v faça:
        // cout << "Chamando query para vertice " << v << endl;
        Tree T1(pair<int,int> (v, vertex_label[v])); 
        Tree T2(pair<int,int> (v, vertex_label[v]));

        // Conjunto dos vértices achados pela busca
        set<int> S = G.bfs_Q(G.getV()[v].getId(), radQ, delta, vertex_label, L, 0);
        
        if(!S.empty()) { // Se S estiver vazio é porque não tem match para v
            Graph sg = getSubgraph(G, S); // Subgrafo induzido pelos vértices de S
            int label1 = verticesOrder1.front(); // Próximo label da esquerda a ser buscado
            verticesOrder1.pop_front(); 
            int label2 = verticesOrder2.front(); // Próximo label da direita a ser buscado
            verticesOrder2.pop_front();

            // Adicionando os vértices na árvore
            for(int u : S) { // Para cada vértice no conjunto S faça:
                if(vertex_label[u] == label1) { // Adiciona somente os que tem label igual ao do próximo vertice
                    //cout << "Adicionando " << u << " na arvore T1" << endl; 
                    pair<int,int> child (u, label1);
                    T1.addChildToNode(child, v);
                } else if (vertex_label[u] == label2) { // Adiciona somente os que tem label igual ao do próximo vertice
                    //cout << "Adicionando " << u << " na arvore T2" << endl; 
                    pair<int,int> child (u, label2);
                    T2.addChildToNode(child, v);
                }
            }
            int label_order = 0;
            query(sg, Q, T1, label1, verticesOrder1, labelsID, label_vertices, radQ - 1, delta, vertex_label, L1, label_order);
            if (Q.getN() % 2 == 0) {
                query(sg, Q, T2, label2, verticesOrder2, labelsID, label_vertices, radQ - 2, delta, vertex_label, L2, label_order);
            } else {
                query(sg, Q, T2, label2, verticesOrder2, labelsID, label_vertices, radQ - 1, delta, vertex_label, L2, label_order);
            }
        }
        m1 = getMatches(T1, radQ);
        if (Q.getN() % 2 == 0) {
            m2 = getMatches(T2, radQ - 1);
        } else {
            m2 = getMatches(T2, radQ);
        }
        for(auto& list1 : m1) {
            for(auto& list2 : m2) {
                list1.merge(list2);
                list1.push_back(v); // Vértice raiz das árvores (Que tem o mesmo label do centro)
                matches.push_back(list1);
            }
        }
    }

    auto end = chrono::steady_clock::now();
    auto time = end - start;
    cout << chrono::duration <double, milli> (time).count() << " ms" << endl;

    if (matches.empty()) {
        cout << "Nao foram encontrados matches" << endl;
    } else {
        for(auto& match : matches) {
            cout << "Match: " << endl;
            for(auto& v : match) {
                cout << v << " " << endl;
            }
            cout << endl;
        }    
    }
    return 0;
}

void query(Graph& G, Graph& Q, Tree& T, int label, deque<int> order, map<string,int> labelsID, vector<list<int>> label_vertices, int r, int delta, map<int,int> vertex_label, vector<list<int>> L, int label_order) {
    //cout << "Query para " << v << endl; 
    for(int w : label_vertices[label]) { // Para cada vértice de G com esse label faça:
        set<int> S = G.bfs_Q(G.getV()[w].getId(), r, delta, vertex_label, L, label_order);
        if (!S.empty()) { // Se S estiver vazio é porque não tem match para esse vértice
            Graph sg = getSubgraph(G, S);
            int next_label = order.front();
            order.pop_front();
            // Adicionando os vértices na árvore
            for(int u : S) { // Para cada vértice no conjunto S faça:
                //cout << "Se " << u << " == " << next << endl; 
                if (vertex_label[u] == next_label) { // Adiciona somente os que tem label igual ao do próximo vertice
                    //cout << "Adicionando " << u << " na arvore" << endl; 
                    pair<int,int> child (u, next_label);
                    T.addChildToNode(child, w);
                } 
            }
            // cout << "Order size: " << order.size() << endl;
            if (order.size() > 0) {
                label_order++;
                //cout << "Label order: " << label_order << endl;
                query(sg, Q, T, next_label, order, labelsID, label_vertices, r-1, delta, vertex_label, L, label_order);
            } 
        } //else cout << "S vazio" << endl;   
    }    
}



list<list<int>> getMatches(Tree& T, int radQ) {
    map<int,int> dist = T.dist(T.getRoot());
    set<int> leafs;
    for(auto& kv : dist) {
        if (kv.second == radQ) {
            leafs.insert(kv.first);
        }
    }
    
    list<list<int>> matches;
    for(int v : leafs) {
        list<int> match;
        while (v != T.getRoot()) {
            match.push_back(v); // Adiciona v no match
            v = T.getParents()[v]; // v = pai de v
        }
        match.push_back(T.getRoot());
        matches.push_back(match);
    }
    return matches;
}

void addToQueue(deque<int>& order, Graph& Q, int v, int u, map<string,int> labelsID) {
    list<int> adjL = Q.getAdjL()[v];
    if(adjL.size() > 1) { // Se esse vértice não for o início ou o fim do caminho
        for(auto& w : adjL) {
            if(w != u) {
                order.push_back(labelsID[Q.getV()[w].getLabel()]);
                addToQueue(order, Q, w, v, labelsID);
            }
        }
    }    
}

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