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


void MaxFlowSolver::validateReviewerNode(const Vertex<nodeInfo>* reviewer) {
    if (reviewer->getAdj().size() > 1) {
        throw std::logic_error("Reviewer node cannot have more than one outgoing edge.");
    }
}


// Note: only values 0 or 1 will be exercised in this project.
void MaxFlowSolver::checkRisk(Result& result, const std::vector<Reviewer>& reviewers, const int generateAssignments) const {

    if (generateAssignments == 1) {
        for (const Reviewer& absent : reviewers) {
            const auto absentNode = flowNetwork->findVertex({REVIEWER, absent.getId()});
            validateReviewerNode(absentNode);
            const Edge<nodeInfo>* absentToSink = absentNode->getAdj()[0]; // edge from the absent reviewer
            int reviewsMissing = absentToSink->getFlow();

            for (const Reviewer& helper : reviewers) {
                // the helper reviewer cannot be the absent, and he must have the same primary expertise as the absent reviewer
                if (helper.getId() != absent.getId() && helper.getPrimaryField() == absent.getPrimaryField()) {

                    const auto helperNode = flowNetwork->findVertex({REVIEWER, helper.getId()});
                    validateReviewerNode(helperNode);
                    const Edge<nodeInfo>* helperToSink = helperNode->getAdj()[0]; // edge from the helper reviewer
                    reviewsMissing -= helperToSink->getCapacity() - helperToSink->getFlow();

                    if (reviewsMissing <= 0) break;
                }
            }

            if (reviewsMissing > 0) result.riskyReviewers.push_back(absent.getId());
        }

    } else if (generateAssignments > 1) {
        const int origFlow = getFlow();

        for (const Reviewer& absent : reviewers) {
            resetAllFlow();

            // Searches the reviewer to be removed
            const auto absentNode = flowNetwork->findVertex({REVIEWER, absent.getId()});
            validateReviewerNode(absentNode);
            Edge<nodeInfo>* absentToSink = absentNode->getAdj()[0]; // edge from the absent reviewer
            const int origCapacity = absentToSink->getCapacity();
            // Updates the flow from that reviewer to the sink to 0, meaning he cannot review any submission
            absentToSink->setCapacity(0);

            // Runs the maxFlow algorithm in the new updated graph
            this->execute();

            // Compares if it changed the maxFlow
            if (getFlow() < origFlow) {
                // If it changed the max flow, then it's a critical edge
                result.riskyReviewers.push_back(absent.getId());
            }

            // reset the edge to the original capacity (MaxReviewsPerReviewer)
            absentToSink->setCapacity(origCapacity);
        }
    }
}


void MaxFlowSolver::checkResults(Result &result, const int riskAnalysis, const std::vector<Reviewer>& reviewers, const int generateAssignments) const {
    outputResults();
    checkMatches(result);
    checkMisses(result);

    if (riskAnalysis > 0)
        checkRisk(result, reviewers, generateAssignments);
}
