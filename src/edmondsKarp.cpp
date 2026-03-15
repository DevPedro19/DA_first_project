// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#include "../data_structures/Graph.h"
#include "../headers/MSTTestAux.h"
#include "../headers/MaxFlow.h"

// Function to test the given vertex 'w' and visit it if conditions are met
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex<T>*> q;
    q.push(s);
    s->setVisited(true);

    while (!q.empty()) {
        Vertex<T>* front = q.front(); q.pop();

        // Check forward primary edges
        for (auto e : front -> getAdj()) {
            if (e->getMatchType() == PRIMARY) {
                testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());
            }
        }

        // We need to fill the primary edges first

        // Check forward secondary edges
        for (auto e : front -> getAdj()) {
            if (e->getMatchType() == SECONDARY) {
                testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());
            }
        }

        // Check Backward, they don't need Matchtype because their purpose is to revert flow
        for (auto e : front -> getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;

    Vertex<T>* curr = t;
    while (curr != s) {
        // Check if it's a forward or back edge
        Edge<T>* edgeTaken =  curr->getPath();

        // It's a forward edge if the destination of that edge is the same as curr
        if (edgeTaken->getDest() == curr) {

            // The value of the bottleneck is the capacity minus the flow of that edge
            f = std::min(f, edgeTaken->getCapacity() - edgeTaken->getFlow());
            curr = edgeTaken->getOrig();
        }
        else {

            // The value of the bottleneck is the flow of that edge because it's the amount of flow that can be reverted
            f = std::min(f, edgeTaken->getFlow());
            curr = edgeTaken->getDest();
        }
    }

    return f;
}

// Function to augment flow along the augmenting path with the given flow value
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    Vertex<T>* curr = t;
    while (curr != s) {
        Edge<T>* edgeTaken = curr->getPath();

        // Check if it's a forward edge
        if (edgeTaken->getDest() == curr) {

            // The augmented flow in a forward edge is achieved by adding the value of the bottleneck to the current flow
            edgeTaken->setFlow(edgeTaken->getFlow() + f);
            curr = edgeTaken->getOrig();
        }
        else {

            // The augmented flow in a backward edge is achieved by subtracting the value of the bottleneck to the current flow
            edgeTaken->setFlow(edgeTaken->getFlow() - f);
            curr = edgeTaken->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
template <class T>
void Graph<T>::runMaxFlowAlgorithm() {
    // Find source and target vertices in the graph
    Vertex<T>* s = this->findVertex(0);
    Vertex<T>* t = this->findVertex(-1);

    for (auto v : this->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (findAugmentingPath(this, s, t)) {
        double bottleneck = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, bottleneck);
    }
}