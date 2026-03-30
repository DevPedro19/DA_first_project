//
// Created by guilhermecunha on 30/03/26.
//
#include "EdmondsKarp.h"

EdmondsKarp::EdmondsKarp(Graph<nodeInfo> *flowNetwork) : MaxFlowAlgorithm(flowNetwork) {}

void EdmondsKarp::testAndVisit(std::queue<Vertex<nodeInfo> *> &q, Edge<nodeInfo> *e, Vertex<nodeInfo> *w, const int residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

void EdmondsKarp::augmentFlowAlongPath(const int f) const {
    const Vertex<nodeInfo>* curr = sink;

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

int EdmondsKarp::findMinResidualAlongPath() const {
    int f = INF;
    const Vertex<nodeInfo>* curr = sink;
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


bool EdmondsKarp::findAugmentingPath() const {
    // Mark all vertices as not visited
    for (const auto v : flowNetwork->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex<nodeInfo>*> q;
    q.push(source);
    source->setVisited(true);

    while (!q.empty()) {
        const Vertex<nodeInfo>* front = q.front(); q.pop();

        // Forward edges
        for (const auto e : front->getAdj())
            testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());

        // Backward edges
        for (const auto e : front->getIncoming())
            testAndVisit(q, e, e->getOrig(), e->getFlow());
    }
    return sink->isVisited();
}


void EdmondsKarp::execute() const {
    for (const auto v : flowNetwork->getVertexSet())
        for (const auto e : v->getAdj())
            e->setFlow(0);

    while (findAugmentingPath()) {
        const int bottleneck = findMinResidualAlongPath();
        augmentFlowAlongPath(bottleneck);
    }
}
