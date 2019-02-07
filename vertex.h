#include <bits/stdc++.h>

using namespace std;

class Vertex {
private:
    int id;
    string label;
public:
    Vertex(int ID, string LABEL);
    int getId();
    void setId(int ID);
    string getLabel();
    void setLabel(string LABEL);
};

Vertex::Vertex(int ID, string LABEL){
    id = ID;
    label = LABEL;
}

int Vertex::getId(){
    return id;
}

void Vertex::setId(int ID){
    id = ID;
}

string Vertex::getLabel(){
    return label;
}

void Vertex::setLabel(string LABEL){
    label = LABEL;
}
