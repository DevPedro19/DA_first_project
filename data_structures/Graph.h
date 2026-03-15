// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>

#include "../headers/Reviewer.h"
#include "../headers/Submission.h"

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

// Used to differentiate edges between submissions and reviewers, as they have different priorities when generating the assignments
enum MatchType { PRIMARY, SECONDARY };

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in);

    T getId() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setVisited(bool visited);
    void setId(T id);
    void setPath(Edge<T> *path);
    Edge<T> * addEdge(Vertex<T> *d, double c, MatchType t);
    bool removeEdge(T in);
    void removeOutgoingEdges();

protected:
    T id;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used to mark if a node has been visited
    Edge<T> *path = nullptr; // used to get the path between two nodes
    std::string primaryField; // used to store the paper's/reviewer's primary field of work

    std::vector<Edge<T> *> incoming; // incoming edges

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double c, MatchType t = PRIMARY);

    Vertex<T> * getDest() const;
    double getCapacity() const;
    Vertex<T> * getOrig() const;
    double getFlow() const;
    MatchType getMatchType() const;

    void setFlow(double flow);
    void setCapacity(double capacity);
    void setMacthType(MatchType t);

protected:
    Vertex<T> * dest; // destination vertex
    double capacity; // max capacity of the edge

    // used for bidirectional edges
    Vertex<T> *orig;

    double flow = 0; // current flow of the edge

    MatchType type;
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(const T &in) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);
    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double c, MatchType t);
    bool removeEdge(const T &source, const T &dest);
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    int getNumVertex() const;

    std::vector<Vertex<T> *> getVertexSet() const;

    void build(std::vector<Submission> submissions, std::vector<Reviewer> reviewers, struct parameters);

    void runMaxFlowAlgorithm();

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;
    /**
    * Auxiliary function to set the "path" field to make a spanning tree.
    */

};


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): id(in) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double c, MatchType t) {
    auto newEdge = new Edge<T>(this, d, c, t);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getId() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
T Vertex<T>::getId() const {
    return this->id;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setId(T id) {
    this->id = id;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *p) {
    this->path = p;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == id) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double c, MatchType t) : orig(orig), dest(dest), capacity(c),  type(t) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getCapacity() const {
    return this->capacity;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template<class T>
MatchType Edge<T>::getMatchType() const {
    return type;
}

template <class T>
void Edge<T>::setFlow(double f) {
    this->flow = f;
}

template<class T>
void Edge<T>::setMacthType(MatchType t) {
    this->type = t;
}

template <class T>
void Edge<T>::setCapacity(double c) {
    this->capacity = c;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return static_cast<int>(i);
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double c, MatchType t) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, c, t);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

template<class T>
void Graph<T>::build(std::vector<Submission> submissions, std::vector<Reviewer> reviewers, struct parameters p) {

    // TODO saber se o id de sub pode ser o mesmo que rev, if so, usar um offset

                                /* CREATE THE NODES */
    // Create the source node ID => 0
    this->addVertex(0);
    // Create the sink node ID => -1
    this->addVertex(-1);
    // Create submission nodes
    for (const Submission& s : submissions) {
        this->addVertex(s.getId());
    }
    // Create review nodes
    for (const Reviewer& r : reviewers) {
        this->addVertex(r.getId());
    }

                        /* CREATE THE EDGES*/
    // Create edge from the source node to submission with capacity MinReviewsPerSubmission
    for (const Submission& s : submissions) {
        this->addEdge(0, s.getId(), p.MinReviewsPerSubmission, PRIMARY);
    }
    // Create edges between submisions and reviewers
    for (const Submission& s : submissions) {
        for (const Reviewer& r : reviewers) {
            switch (p.GenerateAssignments) {
                case 0:
                case 1:
                    // Create only primary edges
                    if (s.getPrimaryField() == r.getPrimaryField()) {
                        this->addEdge(s.getId(), r.getId(), 1, PRIMARY);
                    }

                    break;
                case 2:
                    // Create primary edges
                    if (s.getPrimaryField() == r.getPrimaryField()) {
                        this->addEdge(s.getId(), r.getId(), 1), PRIMARY;
                    }
                    // Create secondary edges

                    // Check if both have a secundary field, else continue
                    if (s.getSecondaryField() == 0 && r.getSecondaryField() == 0) continue;

                    if (s.getSecondaryField() == r.getSecondaryField()) {
                        this->addEdge(s.getId(), r.getId(), 1, SECONDARY);
                    }

                    break;
            }
        }
    }

    // Create edges to sink with capacity equal to the MaxReviewsPerReviewer
    for (const Reviewer& r : reviewers) {
        this->addEdge(r.getId(), -1, p.MaxReviewsPerReviewer,PRIMARY);
    }
}

#endif /* DA_TP_CLASSES_GRAPH */