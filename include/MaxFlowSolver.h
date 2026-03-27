#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.h"
#include "Result.h"

#include <string>
#include <queue>

typedef std::string string;

/**
 * @brief Solves the assignment problem using the Edmonds-Karp maximum flow algorithm.
 */
class MaxFlowSolver {
public:
    /**
     * @brief Constructs a MaxFlowSolver with the given flow network graph.
     * @param g Flow network graph with source and sink vertices.
     */
    explicit MaxFlowSolver(Graph<nodeInfo>* g);

    /**
     * @brief Executes the Edmonds-Karp algorithm to compute maximum flow.
     */
    void execute();

    /**
     * @brief Extracts algorithm results into matches, misses, and risky reviewers.
     * @param result Result object to populate with matches and misses.
     */
    void checkResults(Result& result);

private:
    /**
     * @brief Finds an augmenting path from source to sink using breadth-first search (BFS).
     * @return True if an augmenting path exists, false otherwise.
     */
    bool findAugmentingPath();

    /**
     * @brief Computes the minimum residual capacity along the current augmenting path.
     * @return The bottleneck value (minimum residual capacity).
     */
    double findMinResidualAlongPath();

    /**
     * @brief Augments flow along the current augmenting path by the given amount.
     * Updates both forward and backward edges in the residual graph.
     * @param f Amount of flow to augment (the bottleneck value).
     */
    void augmentFlowAlongPath(double f);

    /**
     * @brief Main algorithm implementation that repeatedly finds augmenting paths and augments flow, while possible,
     * until no more augmenting paths exist, at which point the maximum flow of the network is achieved.
     */
    void edmondsKarp();

    /**
     * @brief Marks a vertex as visited and enqueues it if unvisited and residual capacity exists.
     * Used during breadth-first search to explore reachable vertices in the residual graph.
     * @param q Queue of vertices to visit.
     * @param e Edge leading to the vertex.
     * @param w Destination vertex to potentially mark as visited.
     * @param residual Residual capacity of the edge.
     */
    static void testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, double residual);

    Vertex<nodeInfo>* source, * target;
    Graph<nodeInfo>* flowNetwork;
};

#endif /* MaxFlow_hpp */
