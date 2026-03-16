#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include <queue>
#include "Graph.h"

class MaxFlowSolver {
public:
    explicit MaxFlowSolver(Graph<int>* g);
    void execute();
private:
    bool findAugmentingPath();
    double findMinResidualAlongPath();
    void augmentFlowAlongPath(double f);
    void edmondsKarp();

    static void testAndVisit(std::queue<Vertex<int>*> &q, Edge<int> *e, Vertex<int> *w, double residual);

    Vertex<int>* source, * target;
    Graph<int>* flowNetwork;
};

#endif /* MaxFlow_hpp */
