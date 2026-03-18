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
void MaxFlowSolver::testAndVisit(std::queue<Vertex<nodeInfo>*> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
bool MaxFlowSolver::findAugmentingPath() {
    // Mark all vertices as not visited
    for (auto v : flowNetwork->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex<nodeInfo>*> q;
    q.push(source);
    source->setVisited(true);

    while (!q.empty()) {
        Vertex<nodeInfo>* front = q.front(); q.pop();

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
double MaxFlowSolver::findMinResidualAlongPath() {
    double f = INF;
    Vertex<nodeInfo>* curr = target;
    while (curr != source) {
        // Check if it's a forward or back edge
        Edge<nodeInfo>* e = curr->getPath();

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
void MaxFlowSolver::augmentFlowAlongPath(double f) {
    Vertex<nodeInfo>* curr = target;

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
void MaxFlowSolver::edmondsKarp() {
    for (auto v : flowNetwork->getVertexSet())
        for (auto e : v->getAdj())
            e->setFlow(0);

    while (findAugmentingPath()) {
        double bottleneck = findMinResidualAlongPath();
        augmentFlowAlongPath(bottleneck);
    }
}

void MaxFlowSolver::execute() {
    edmondsKarp();
}

void MaxFlowSolver::checkResults(Result &result) {
    // Check matching submissions and reviewers
    for (auto v : flowNetwork->getVertexSet()) {
        if (v->getInfo().type == SUBMISSION) {
            for (auto e : v->getAdj()) {
                if (e->getCapacity() - e->getFlow() == 0) {
                    result.matches.push_back({v->getInfo().id, e->getDest()->getInfo().id, e->getDomain()});
                }
            }
        }
    }
    // Check if submissions have missing Reviews
    for (auto e : source->getAdj()) {
        int missingReviews = e->getCapacity() - e->getFlow();
        if (missingReviews > 0) {
            result.misses.push_back({e->getDest()->getInfo().id, e->getDomain(), missingReviews});
        }
    }
}
