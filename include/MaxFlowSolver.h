#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.h"
#include "Result.h"

#include <string>
#include <queue>

typedef std::string string;
class MaxFlowSolver {
public:
    explicit MaxFlowSolver(Graph<nodeInfo>* g);
    void execute();
    void checkResults(Result& result, int riskAnalysis, int r, int maxRpR);
private:
    bool findAugmentingPath();
    double findMinResidualAlongPath();
    void augmentFlowAlongPath(double f);
    void edmondsKarp();
    double getFlow();
    void resetAllFlow();

    static void testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, double residual);

    Vertex<nodeInfo>* source, * target;
    Graph<nodeInfo>* flowNetwork;
};

#endif /* MaxFlow_hpp */
