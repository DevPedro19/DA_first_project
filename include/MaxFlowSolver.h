#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.h"
#include "Result.h"


#include "MaxFlowAlgorithm.h"

/**
 * @brief Solves the assignment problem using a maximum flow algorithm.
 */
class MaxFlowSolver {
public:
    /**
     * @brief Constructs a MaxFlowSolver with the given flow network graph.
     * @param g Flow network graph with source and sink vertices.
     * @param maxFlowAlgorithm The maximum flow algorithm to be executed on the flow network, which must be a specific implementation of a maximum flow algorithm that inherits from the MaxFlowAlgorithm class.
     */
    explicit MaxFlowSolver(Graph<nodeInfo>* g, MaxFlowAlgorithm* maxFlowAlgorithm);

    /**
     * @brief Executes the maximum flow algorithm to compute maximum flow.
     */
    void execute() const;

    /**
     * @brief Outputs the residual graph after running the max flow algorithm, showing the flow and capacity of each edge.
     */
    void outputResults() const;

    /**
     * @brief Checks the residual graph after running the max flow algorithm to populate the result object with the matches, which are the edges from submissions to reviewers that have no residual capacity.
     * @param result The result object to be populated with the matches.
     */
    void checkMatches(Result& result) const;

    /**
     * @brief Checks the residual graph after running the max flow algorithm to populate the result object with the misses, which are the edges from the source to submissions that have residual capacity.
     * @param result The result object to be populated with the misses.
     */
    void checkMisses(Result& result) const;


    /**
     * @brief Checks the risk of each reviewer by temporarily preventing him from reviewing any submission and checking if the maximum flow decreases.
     * @param result The result object to be populated with the risky reviewers.
     * @param reviewers Array of all the reviewers of the flow network.
     */
    void checkRisk(Result& result, const std::vector<Reviewer>& reviewers) const;

    /**
     * @brief Calls auxiliary functions to populate the result object with the matches, misses and risky reviewers, if risk analysis is required.
     * @param result Result object to be populated with the matches, misses and risky reviewers, if risk analysis is required.
     * @param riskAnalysis Indicates whether risk analysis is required (> 0) or not (0).
     * @param reviewers Array of reviewers, which is used to check the risk of each reviewer in case risk analysis is required.
     */
    void checkResults(Result& result, int riskAnalysis, const std::vector<Reviewer>& reviewers) const;
private:
    /**
     * @brief Computes the total flow from the source to the sink by summing the flow of all edges outgoing from the source.
     * @return The total flow value, which represents the maximum flow of the network after running the max flow algorithm.
     */
    [[nodiscard]] int getFlow() const;

    /**
     * @brief Sets the flow of all the edges in the flow network to 0.
     */
    void resetAllFlow() const;

    /**
     * @brief The source node of the flow network.
     */
    Vertex<nodeInfo>* source;

    /**
     * @brief The flow network bipartite graph.
     */
    Graph<nodeInfo>* flowNetwork;

    /**
     * @brief The maximum flow algorithm to be executed on the flow network, which must be a specific implementation of a maximum flow algorithm that inherits from the MaxFlowAlgorithm class.
     */
    MaxFlowAlgorithm* maxFlowAlgorithm_;
};

#endif /* MaxFlow_hpp */
