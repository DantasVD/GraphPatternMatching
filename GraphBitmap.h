#include <bits/stdc++.h>

using namespace std;
class GraphBitmap {

private:
    int N; // Número de vértices
    vector<Bitmap*> bitMapList; // Lista contendo as variáveis bitmap
    Vertex** V; // Array contendo os vértices
public:
    // Construtor
    GraphBitmap(int vertexNumber, vector<Bitmap*> bitmapList);

    //Recebe um vértice e retorna o valor do boolean do bitmap dele
    bool getValue(Vertex v);

    //Retorna todos os vértices que tem como valor de boolean True
    vector<Vertex*> getAllTrue();

    //Retorna o bitmap da intersecção do objeto com o graphBitmap passado
    GraphBitmap intersection(GraphBitmap *other);

    //Retorna o bitmap da união do objeto com o graphBitmap passado
    GraphBitmap join(GraphBitmap *other);
};

GraphBitmap::GraphBitmap(int vertexNumber, vector<Bitmap*> bitmapList){
    N = vertexNumber;
    bitMapList.resize(N);
    V = (Vertex**)malloc(N*sizeof(Vertex*));
    for(int i = 0; i < bitmapList.size(); i++){
        Bitmap b = *bitmapList[i];
        int bid = b.getVertex()->getId();
        V[bid] = b.getVertex();
        bitMapList[bid] = bitmapList[i];
    }
}

bool GraphBitmap::getValue(Vertex v){
    int vid = v.getId();
    Bitmap b = *bitMapList[vid];
    return b.getBool();
}

vector<Vertex*> GraphBitmap::getAllTrue(){
    vector<Vertex*> v;
    for(int i = 0; i < N; i++){
        if(bitMapList[i]->getBool()){
            v.push_back(V[i]);
        }
    }
    return v;
}

GraphBitmap GraphBitmap::intersection(GraphBitmap* other){
    vector<Bitmap*> newVB;
    for(int i = 0; i < N; i++){
        Vertex* vex = bitMapList[i]->getVertex();
        Bitmap* b = new Bitmap(vex,false);
        newVB.push_back(b);
    }
    vector<Vertex*> allTrue = other->getAllTrue();
    for(int i = 0; i < allTrue.size(); i++){
        newVB[allTrue[i]->getId()]->setBool(true);
    }
    for(int i = 0; i < N; i++){
        bool nbi = newVB[i]->getBool();
        bool obi = bitMapList[i]->getBool();
        newVB[i]->setBool(nbi && obi);
    }
    GraphBitmap* newGB = new GraphBitmap(N,newVB);
    return *newGB;
}

GraphBitmap GraphBitmap::join(GraphBitmap* other){
    vector<Bitmap*> newVB;
    for(int i = 0; i < N; i++){
        Vertex* vex = bitMapList[i]->getVertex();
        bool vexbit = bitMapList[i]->getBool();
        Bitmap* b = new Bitmap(vex,vexbit);
        newVB.push_back(b);
    }
    vector<Vertex*> allTrue = other->getAllTrue();
    for(int i = 0; i < allTrue.size(); i++){
        newVB[allTrue[i]->getId()]->setBool(true);
    }
    GraphBitmap* newGB = new GraphBitmap(N,newVB);
    return *newGB;
}
