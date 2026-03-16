#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include <queue>
#include <limits>
#include "../data_structures/Graph.h"

template <class T>
class MaxFlowSolver {
public:
    void execute(Graph<T>* g, int source, int target);
private:
    bool findAugmentingPath(Graph<T>* g, Vertex<T> *s, Vertex<T> *t);
    double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
    void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f);
    void edmondsKarp(Graph<T>* g, int source, int target);
    void testAndVisit(std::queue<Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);
};

#endif /* MaxFlow_hpp */

