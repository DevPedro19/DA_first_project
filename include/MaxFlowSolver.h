#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include <queue>
#include "Graph.h"

class MaxFlowSolver {
public:
    explicit MaxFlowSolver(Graph<nodeInfo>* g);
    void execute();
private:
    bool findAugmentingPath();
    double findMinResidualAlongPath();
    void augmentFlowAlongPath(double f);
    void edmondsKarp();

    static void testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, double residual);

    Vertex<nodeInfo>* source, * target;
    Graph<nodeInfo>* flowNetwork;
};

#endif /* MaxFlow_hpp */
