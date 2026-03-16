#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include <queue>
#include "Graph.h"
#include <string>

typedef std::string string;

class MaxFlowSolver {
public:
    explicit MaxFlowSolver(Graph* g);
    void execute();
private:
    bool findAugmentingPath();
    double findMinResidualAlongPath();
    void augmentFlowAlongPath(double f);
    void edmondsKarp();

    static void testAndVisit(std::queue<Vertex*> &q, Edge *e, Vertex *w, double residual);

    Vertex* source, * target;
    Graph* flowNetwork;
};

#endif /* MaxFlow_hpp */
