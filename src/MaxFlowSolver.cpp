#include "MaxFlowSolver.h"
#include <queue>
#include "Graph.h"

MaxFlowSolver::MaxFlowSolver(Graph<nodeInfo>* g, MaxFlowAlgorithm* maxFlowAlgorithm) {
    source = g->findVertex({SOURCE, 0});
    flowNetwork = g;
    maxFlowAlgorithm_ = maxFlowAlgorithm;
}


void MaxFlowSolver::execute() const {
    maxFlowAlgorithm_->execute();
}

int MaxFlowSolver::getFlow() const {
    int flow = 0;
    for (const auto e : source->getAdj()) {
        flow += e->getFlow();
    }
    return flow;
}

void MaxFlowSolver::resetAllFlow() const {
    for (const auto v : flowNetwork->getVertexSet())
        for (const auto e : v->getAdj())
            e->setFlow(0);
}


void MaxFlowSolver::checkMatches(Result& result) const {
    for (const auto v : flowNetwork->getVertexSet()) {
        if (v->getInfo().type == SUBMISSION) {
            for (const auto e : v->getAdj()) {
                if (e->getCapacity() - e->getFlow() == 0) { // there is a match
                    result.matches.push_back({v->getInfo().id, e->getDest()->getInfo().id, e->getDomain()});
                }
            }
        }
    }
}


void MaxFlowSolver::checkMisses(Result &result) const {
    for (const auto e : source->getAdj()) {
        int missingReviews = e->getCapacity() - e->getFlow();
        if (missingReviews > 0) {
            result.misses.push_back({e->getDest()->getInfo().id, e->getDomain(), missingReviews});
        }
    }
}


void MaxFlowSolver::outputResults() const {
    std::cout << "\n============ Residual Graph ============\n" << std::endl;
    for (const auto v : flowNetwork->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            std::cout << enumToString(v->getInfo().type) << " " << v->getInfo().id << " -- "
            << e->getFlow() << "/" << e->getCapacity() << " --> " << enumToString(e->getDest()->getInfo().type)
            << " " << e->getDest()->getInfo().id << std::endl;
        }
    }
}


// Note: only values 0 or 1 will be exercised in this project.
void MaxFlowSolver::checkRisk(Result& result, const std::vector<Reviewer>& reviewers) const {
    const int origFlow = getFlow();

    for (const Reviewer& reviewer : reviewers) {
        resetAllFlow();

        // Searches the reviewer to be removed
        const auto absent = flowNetwork->findVertex({REVIEWER, reviewer.getId()});
        if (absent->getAdj().size() > 1) {
            throw std::logic_error("Reviewer node cannot have more than one outgoing edge.");
        }
        Edge<nodeInfo>* absentToSink = absent->getAdj()[0]; // edge from the absent reviewer
        const int origCapacity = absentToSink->getCapacity();
        // Updates the flow from that reviewer to the sink to 0, meaning he cannot review any submission
        absentToSink->setCapacity(0);

        // Runs the maxFlow algorithm in the new updated graph
        this->execute();

        // Compares if it changed the maxFlow
        if (getFlow() < origFlow) {
            // If it changed the max flow, then it's a critical edge
            result.riskyReviewers.push_back(reviewer.getId());
        }

        // reset the edge to the original capacity (MaxReviewsPerReviewer)
        absentToSink->setCapacity(origCapacity);
    }
}


void MaxFlowSolver::checkResults(Result &result, const int riskAnalysis, const std::vector<Reviewer>& reviewers) const {
    outputResults();
    checkMatches(result);
    checkMisses(result);

    if (riskAnalysis > 0)
        checkRisk(result, reviewers);
}
