#include <bits/stdc++.h>

using namespace std;

class Bitmap {
private:
    Vertex* vertice;
    bool bit;
public:
    //Construtor
    Bitmap(Vertex* ver, bool value);
    //gets e sets
    void setBool(bool value);
    bool getBool();
    Vertex* getVertex();
};

Bitmap::Bitmap(Vertex* ver, bool value){
    vertice = ver;
    bit = value;
}

void Bitmap::setBool(bool value){
    bit = value;
}

bool Bitmap::getBool(){
    return bit;
}

Vertex* Bitmap::getVertex(){
    return vertice;
}
