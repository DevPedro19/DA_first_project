#include "MaxFlowSolver.h"
#include <limits>
#include <queue>
#include <algorithm>
#include "Graph.h"

MaxFlowSolver::MaxFlowSolver(Graph<nodeInfo>* g) {
    source = g->findVertex({SOURCE, 0});
    target = g->findVertex({SINK, -1});
    flowNetwork = g;
}

// Function to test the given vertex 'w' and visit it if conditions are met
void MaxFlowSolver::testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, int residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
bool MaxFlowSolver::findAugmentingPath() const {
    // Mark all vertices as not visited
    for (auto v : flowNetwork->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex<nodeInfo>*> q;
    q.push(source);
    source->setVisited(true);

    while (!q.empty()) {
        const Vertex<nodeInfo>* front = q.front(); q.pop();

        // Forward edges
        for (auto e : front->getAdj())
            testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());

        // Backward edges
        for (auto e : front->getIncoming())
            testAndVisit(q, e, e->getOrig(), e->getFlow());
    }
    return target->isVisited();
}


// Function to find the minimum residual capacity along the augmenting path
int MaxFlowSolver::findMinResidualAlongPath() const {
    int f = INF;
    const Vertex<nodeInfo>* curr = target;
    while (curr != source) {
        // Check if it's a forward or back edge
        const Edge<nodeInfo>* e = curr->getPath();

        // It's a forward edge if the destination of that edge is the same as curr
        if (e->getDest() == curr) {

            // The value of the bottleneck is the capacity minus the flow of that edge
            f = std::min(f, e->getCapacity() - e->getFlow());
            curr = e->getOrig();

            // It's a backward edge
        } else {

            // The value of the bottleneck is the flow of that edge because it's the amount of flow that can be reverted
            f = std::min(f, e->getFlow());
            curr = e->getDest();
        }
    }
    return f;
}

// Function to augment flow along the augmenting path with the given flow value
void MaxFlowSolver::augmentFlowAlongPath(int f) const {
    const Vertex<nodeInfo>* curr = target;

    while (curr != source) {
        Edge<nodeInfo>* e = curr->getPath();

        // Check if it's a forward edge
        if (e->getDest() == curr) {

            // The augmented flow in a forward edge is achieved by adding the value of the bottleneck to the current flow
            e->setFlow(e->getFlow() + f);
            curr = e->getOrig();
        } else {

            // The augmented flow in a backward edge is achieved by subtracting the value of the bottleneck to the current flow
            e->setFlow(e->getFlow() - f);
            curr = e->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
void MaxFlowSolver::edmondsKarp() const {
    for (auto v : flowNetwork->getVertexSet())
        for (auto e : v->getAdj())
            e->setFlow(0);

    while (findAugmentingPath()) {
        int bottleneck = findMinResidualAlongPath();
        augmentFlowAlongPath(bottleneck);
    }
}

void MaxFlowSolver::execute() const {
    edmondsKarp();
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
