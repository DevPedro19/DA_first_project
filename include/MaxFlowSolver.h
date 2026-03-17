#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.h"
#include "Result.h"

#include <queue>
#include <string>

typedef std::string string;
class MaxFlowSolver {
public:
    explicit MaxFlowSolver(Graph<T>* g);
    void execute();
    void checkResults(Result& result);
private:
    bool findAugmentingPath();
    double findMinResidualAlongPath();
    void augmentFlowAlongPath(double f);
    void edmondsKarp();

    static void testAndVisit(std::queue<Vertex*> &q, Edge *e, Vertex *w, double residual);

    Vertex<T>* source, * target;
    Graph<T>* flowNetwork;
};

#endif /* MaxFlow_hpp */
