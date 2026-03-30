//
// Created by guilhermecunha on 30/03/26.
//

#ifndef ORGANIZATIONALTOOL_EDMONDSKARP_H
#define ORGANIZATIONALTOOL_EDMONDSKARP_H

#include "MaxFlowAlgorithm.h"
#include <queue>
#include <limits>

#define INF std::numeric_limits<int>::max();

/**
 * @brief Implements the Edmonds-Karp algorithm for computing maximum flow in a flow network.
 */
class EdmondsKarp : public MaxFlowAlgorithm {
public:
    /**
     * @brief Constructor for the EdmondsKarp class, which initializes the flow network, source and sink nodes for the algorithm.
     * @param flowNetwork The flow network graph on which the Edmonds-Karp algorithm will be executed.
     */
    explicit EdmondsKarp(Graph<nodeInfo>* flowNetwork);

    /**
     * @brief Executes the Edmonds Karp algorithm that repeatedly finds augmenting paths and augments flow, while possible, until no more augmenting paths exist, at which point the maximum flow of the network is achieved.
     * @par Complexity
     * Time: O(V * E^2)
     */
    void execute() const override;

private:

    /**
     * @brief Marks a vertex as visited and enqueues it if unvisited and residual capacity exists.
     * Used during breadth-first search to explore reachable vertices in the residual graph.
     * @param q Queue of vertices to visit.
     * @param e Edge leading to the vertex.
     * @param w Destination vertex to potentially mark as visited.
     * @param residual Residual capacity of the edge.
     * @par Complexity
     * Time: O(1)
     */
    static void testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, int residual);

    /**
     * @brief Finds an augmenting path from source to sink using breadth-first search (BFS).
     * @return True if an augmenting path exists, false otherwise.
     * @par Complexity
     * Time: O(V + E)
     */
    [[nodiscard]] bool findAugmentingPath() const;

    /**
     * @brief Computes the minimum residual capacity along the current augmenting path.
     * @return The bottleneck value (minimum residual capacity).
     * @par Complexity
     * Time: O(V)
     */
    [[nodiscard]] int findMinResidualAlongPath() const;

    /**
     * @brief Augments flow along the current augmenting path by the given amount.
     * Updates both forward and backward edges in the residual graph.
     * @param f Amount of flow to augment (the bottleneck value).
     * @par Complexity
     * Time: O(V)
     */
    void augmentFlowAlongPath(int f) const;
};

#endif //ORGANIZATIONALTOOL_EDMONDSKARP_H
