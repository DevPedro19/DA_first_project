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
double MaxFlowSolver::findMinResidualAlongPath() const {
    double f = INF;
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
void MaxFlowSolver::augmentFlowAlongPath(double f) const {
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
        double bottleneck = findMinResidualAlongPath();
        augmentFlowAlongPath(bottleneck);
    }
}

void MaxFlowSolver::execute() const {
    edmondsKarp();
}

double MaxFlowSolver::getFlow() const {
    double flow = 0;
    for (const auto e : source->getAdj()) {
        flow += e->getFlow();
    }
    return flow;
}

// usar um BFS

void MaxFlowSolver::resetAllFlow() const{
    for(auto v : flowNetwork->getVertexSet()){
        v->setVisited(false);
    }
    std::queue<Vertex<nodeInfo>*> q;
    q.push(source);
    source->setVisited(true);
    while(!q.empty()) {
        auto u = q.front(); q.pop();
        u->setVisited(true);
        for (auto e : u->getAdj()) {
            auto v = e->getDest();
            if (!v->isVisited()) {
                e->setFlow(0);
                q.push(v);
            }
        }
    }
}

void MaxFlowSolver::checkResults(Result &result, int riskAnalysis, int r, int maxRpR) const{
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
        auto missingReviews = static_cast<int>(e->getCapacity() - e->getFlow());
        if (missingReviews > 0) {
            result.misses.push_back({e->getDest()->getInfo().id, e->getDomain(), missingReviews});
        }
    }

    for (auto v : flowNetwork->getVertexSet()) {
        for (auto e : v->getAdj()) {
            std::cout << enumToString(v->getInfo().type) << " " << v->getInfo().id << " -- "
            << e->getFlow() << "/" << e->getCapacity() << " --> " << enumToString(e->getDest()->getInfo().type)
            << " " << e->getDest()->getInfo().id << std::endl;
        }
    }

    if (riskAnalysis == 0) return;

    double flow = getFlow();

    //Only values 0 or 1 will be exercised in this project.

    for (int i = 1; i <= r; i++) {
        // Resets all flows to 0
        resetAllFlow();

        // Searchs the reviewer to be removed
        auto v = flowNetwork->findVertex({REVIEWER, i});
        // Updates the flow from that reviewer to the sink to 0, as if it was eliminated
        v->getAdj()[0]->setCapacity(0);

        // Runs the maxFlow algorithm in the new updated graph
        execute();

        // Compares if it changed the maxFlow
        if (flow != getFlow()) {
            // If it changed the max flow, then it's a critical edge
            result.riskyReviewers.push_back(i);
        }

        // reset the edge to the normal capacity
        v->getAdj()[0]->setCapacity(maxRpR);
    }
}
