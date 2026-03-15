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
        // Para as forward edges
        for (auto e : front -> getAdj()) {
            // Segundo a função testAndVisit verificamos se podemos usar a aresta
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        // Para as backward edges
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
        // Verificar se estamos a percorrer uma forward edge ou reverse edge
        Edge<T>* edgeTaken =  curr->getPath();
        // Se a partir da edgeTaken chegamos ao vértice atual, forward edge
        if (edgeTaken->getDest() == curr) {
            // O valor do bottleneck é dado pela capacidade menos o flow
            f = std::min(f, edgeTaken->getWeight() - edgeTaken->getFlow());
            curr = edgeTaken->getOrig();
        }
        else {
            // O valor do bottleneck é dado exclusivamente pelo flow
            // Capacidade residual da backwards edge é igual ao flow da forward edge
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
        // Verificar se estamos a percorrer uma forward edge
        if (edgeTaken->getDest() == curr) {
            // Temos de adicionar ao valor do fluxo da forward edge o valor do bottleneck
            // que é a quantidade máxima de fluxo que pode ser adicionada
            edgeTaken->setFlow(edgeTaken->getFlow() + f);
            curr = edgeTaken->getOrig();
        }
        else {
            // Subtrair o valor do bottleneck
            edgeTaken->setFlow(edgeTaken->getFlow() - f);
            curr = edgeTaken->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
template <class T>
void edmondsKarp(Graph<T> *g, int source, int target) {
    // Find source and target vertices in the graph
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);

    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (findAugmentingPath(g, s, t)) {
        double bottleneckVal = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, bottleneckVal);
    }

}
