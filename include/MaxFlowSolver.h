#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.h"
#include "Result.h"

#include <queue>

/**
 * @brief Solves the assignment problem using a maximum flow algorithm.
 */
class MaxFlowSolver {
public:
    /**
     * @brief Constructs a MaxFlowSolver with the given flow network graph.
     * @param g Flow network graph with source and sink vertices.
     */
    explicit MaxFlowSolver(Graph<nodeInfo>* g);

    /**
     * @brief Executes the maximum flow algorithm to compute maximum flow.
     */
    void execute() const;

    void outputResults() const;

    void checkMatches(Result& result) const;

    void checkMisses(Result& result) const;

    void checkRisk(Result& result, const std::vector<Reviewer>& reviewers) const;

    /**
     * @brief Checks the residual graph after running the max flow algorithm to populate the result object with the matches, misses and risky reviewers, if risk analysis is required.
     * @param result Result object to be populated with the matches, misses and risky reviewers, if risk analysis is required.
     * @param riskAnalysis Indicates whether risk analysis is required (> 0) or not (0).
     * @param reviewers Array of reviewers, which is used to check the risk of each reviewer in case risk analysis is required.
     */
    void checkResults(Result& result, int riskAnalysis, const std::vector<Reviewer>& reviewers) const;
private:
    /**
     * @brief Finds an augmenting path from source to sink using breadth-first search (BFS).
     * @return True if an augmenting path exists, false otherwise.
     */
    [[nodiscard]] bool findAugmentingPath() const;

    /**
     * @brief Computes the minimum residual capacity along the current augmenting path.
     * @return The bottleneck value (minimum residual capacity).
     */
    [[nodiscard]] int findMinResidualAlongPath() const;

    /**
     * @brief Augments flow along the current augmenting path by the given amount.
     * Updates both forward and backward edges in the residual graph.
     * @param f Amount of flow to augment (the bottleneck value).
     */
    void augmentFlowAlongPath(int f) const;

    /**
     * @brief Main algorithm implementation that repeatedly finds augmenting paths and augments flow, while possible,
     * until no more augmenting paths exist, at which point the maximum flow of the network is achieved.
     */
    void edmondsKarp() const;
    [[nodiscard]] int getFlow() const;
    void resetAllFlow() const;

    /**
     * @brief Marks a vertex as visited and enqueues it if unvisited and residual capacity exists.
     * Used during breadth-first search to explore reachable vertices in the residual graph.
     * @param q Queue of vertices to visit.
     * @param e Edge leading to the vertex.
     * @param w Destination vertex to potentially mark as visited.
     * @param residual Residual capacity of the edge.
     */
    static void testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, int residual);

    Vertex<nodeInfo>* source, * target;
    Graph<nodeInfo>* flowNetwork;
};

#endif /* MaxFlow_hpp */
