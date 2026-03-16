// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

// Template definitions have been moved to headers/MaxFlowSolver.h
// because C++ templates must be fully defined in headers (visible at instantiation).
#include "../headers/MaxFlowSolver.h"

template<class T>
// Function to test the given vertex 'w' and visit it if conditions are met
void MaxFlowSolver<T>::testAndVisit(std::queue<Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (!w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}


template <class T>
// Function to find an augmenting path using Breadth-First Search
bool MaxFlowSolver<T>::findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for (auto v : g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<Vertex<T>*> q;
    q.push(s);
    s->setVisited(true);

    while (!q.empty()) {
        Vertex<T>* front = q.front(); q.pop();

        // Forward primary edges first
        for (auto e : front->getAdj())
            if (e->getMatchType() == PRIMARY)
                testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());

        // Forward secondary edges
        for (auto e : front->getAdj())
            if (e->getMatchType() == SECONDARY)
                testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());

        // Backward edges (flow reversal)
        for (auto e : front->getIncoming())
            testAndVisit(q, e, e->getOrig(), e->getFlow());
    }
    return t->isVisited();
}

template <class T>
// Function to find the minimum residual capacity along the augmenting path
double MaxFlowSolver<T>::findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    Vertex<T>* curr = t;
    while (curr != s) {
        // Check if it's a forward or back edge
        Edge<T>* e = curr->getPath();

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

template <class T>
// Function to augment flow along the augmenting path with the given flow value
void MaxFlowSolver<T>::augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    Vertex<T>* curr = t;

    while (curr != s) {
        Edge<T>* e = curr->getPath();

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

template <class T>
// Main function implementing the Edmonds-Karp algorithm
void MaxFlowSolver<T>::edmondsKarp(Graph<T>* g, int source, int target) {
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);

    for (auto v : g->getVertexSet())
        for (auto e : v->getAdj())
            e->setFlow(0);

    while (findAugmentingPath(g, s, t)) {
        double bottleneck = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, bottleneck);
    }
}

template<class T>
void MaxFlowSolver<T>::execute(Graph<T> *g, int source, int target) {
    edmondsKarp(g, source, target);
}
