//
// Created by guilhermecunha on 30/03/26.
//

#ifndef ORGANIZATIONALTOOL_MAXFLOWALGORITHM_H
#define ORGANIZATIONALTOOL_MAXFLOWALGORITHM_H
#include "Graph.h"

/**
 * @brief Abstract base class for maximum flow algorithms.
 */
class MaxFlowAlgorithm {
public:
    /**
     * @brief Constructor for the MaxFlowAlgorithm class, which initializes the flow network, source and sink nodes for the algorithm.
     * @param flowNetwork The flow network graph on which the maximum flow algorithm will be executed.
     */
    explicit MaxFlowAlgorithm(Graph<nodeInfo>* flowNetwork);

    /**
     * @brief Pure virtual function to execute the maximum flow algorithm, which should be implemented for specific algorithms.
     */
    virtual void execute() const = 0;

    /**
     * @brief Virtual destructor for the MaxFlowAlgorithm class.
     */
    virtual ~MaxFlowAlgorithm() = default;

protected:
    /**
     * @brief The flow network graph.
     */
    Graph<nodeInfo>* flowNetwork;

    /**
     * @brief The source node of the flow network.
     */
    Vertex<nodeInfo>* source;

    /**
     * @brief The sink node of the flow network.
     */
    Vertex<nodeInfo>* sink;
};
#endif //ORGANIZATIONALTOOL_MAXFLOWALGORITHM_H
