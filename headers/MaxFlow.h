#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "../data_structures/Graph.h"


/**
 *
 * @tparam T datatype associado ao grafo (por exemplo ID do nó)
 * @param g grafo onde se pretende calcular o fluxo máximo
 * @param source ID do nó fonte, que envia fluxo infinito à rede
 * @param target ID do nó sumidouro, que recebe fluxo infinito da rede
 */
template <class T>
void edmondsKarp(Graph<T>*g, int source, int target);
template <class T>
bool findAugmentingPath(Graph<T>* g, Vertex<T> *s, Vertex<T> *t);
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f);

#endif /* MaxFlow_hpp */