#pragma once
#ifndef graphBitmap_H
#define graphBitmap_H

//
// Created by T-Gamer on 24/01/2019.
//

#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "graph.h"


class Bitmap {
private:
    Vertex vertice;
    bool bit;
public:
    //Construtor
    Bitmap(Vertex *ver, bool value);
    //gets e sets
    void setBool(bool value);
    bool getBool();
    Vertex getVertex();
    
    
    
};

class GraphBitmap {

private:
    int N; // Número de vértices
    list<bitmap> *bitMapList; // Lista contendo as variáveis bitmap
    Vertex *V; // Array contendo os vértices
public:
    // Construtor
    GraphBitmap(int vertexNumber, list<Bitmap> *bitmapList);
    
    //Recebe um vértice e retorna o valor do boolean do bitmap dele
    bool getValue(Vertex v);
    
    //Retorna todos os vértices que tem como valor de boolean True
    list<Vertex*> getAllTrue();
    
    //Retorna o bitmap da intersecção do objeto com o graphBitmap passado
    GraphBitmap intersection(GraphBitmap* graphBitmap);
    
    
};

#endif //grapBitmap_H