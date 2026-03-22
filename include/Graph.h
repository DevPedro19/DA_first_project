// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <vector>
#include <queue>
#include "Data.h"

enum type {
    SOURCE,
    SINK,
    SUBMISSION,
    REVIEWER
};

inline std::string enumToString(const type t) {
    switch (t) {
        case SOURCE: return "SOURCE";
        case SINK: return "SINK";
        case SUBMISSION: return "SUBMISSION";
        case REVIEWER: return "REVIEWER";
    }
    return "";
}

struct nodeInfo {
    enum type type;
    int id;
};


template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

// Offset to avoid reviewers and submissions with the same id

/************************* Vertex  **************************/
template <class T>
class Vertex {
public:
    Vertex(const T &in);

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    Edge<T>* getPath() const;
    std::vector<Edge<T>*> getIncoming() const;

    void setVisited(bool visited);

    void setInfo(T info);
    void setPath(Edge<T> *path);
    Edge<T>* addEdge(Vertex<T> *d, double capacity, int domain = 0);
    bool removeEdge(const T &id);
    void removeOutgoingEdges();

protected:
    T info_;                // name of the node
    std::vector<Edge<T>*> adj;  // outgoing edges

    // auxiliary fields
    bool visited_ = false; // used to mark if a node has been visited
    Edge<T> *path_ = nullptr; // used to get the path between two nodes

    std::vector<Edge<T>*> incoming; // incoming edges

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double capacity, int domain=0);

    Vertex<T>* getDest() const;
    double getCapacity() const;
    Vertex<T>* getOrig() const;
    double getFlow() const;
    int getDomain() const;

    void setFlow(double flow);
    void setDomain(int domain);
    void setCapacity(double capacity);

protected:
    Vertex<T>*dest; // destination vertex
    double capacity_; // max capacity of the edge

    int domain_;

    // used for bidirectional edges
    Vertex<T> *orig;

    double flow_ = 0; // current flow of the edge

};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    Graph(Data &data);
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T>* findVertex(const T &in) const;
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
    bool addEdge(const T &sourc, const T &dest, double c, int domain = 0);
    bool removeEdge(const T &source, const T &dest);

    int getNumVertex() const;

    std::vector<Vertex<T>*> getVertexSet() const;

protected:
    std::vector<Vertex<T>*> vertexSet;    // vertex set

};

//
// Created by Gabriel Magalhaes on 16/03/2026.
//
#include "Graph.h"

/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(const T &in): info_(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T>* Vertex<T>::addEdge(Vertex<T> *d, double capacity, int domain) {
    auto newEdge = new Edge<T>(this, d, capacity, domain);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */

template<class T>
bool Vertex<T>::removeEdge(const T &id) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getInfo() == id) {
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
template<class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template<class T>
T Vertex<T>::getInfo() const {
    return this->info_;
}

template<class T>
std::vector<Edge<T> *> Vertex<T>::getAdj() const {
    return adj;
}

template<class T>
bool Vertex<T>::isVisited() const {
    return this->visited_;
}

template<class T>
Edge<T>* Vertex<T>::getPath() const {
    return this->path_;
}

template<class T>
std::vector<Edge<T>*> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template<class T>
void Vertex<T>::setVisited(const bool visited) {
    this->visited_ = visited;
}

template<class T>
void Vertex<T>::setInfo(T info) {
    this->info_ = info;
}

template<class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path_ = path;
}

template<class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove only the matching edge pointer from the incoming list.
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if (*it == edge) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template<class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double capacity, int domain) : dest(dest), capacity_(capacity), orig(orig), domain_(domain) {}

template<class T>
Vertex<T>* Edge<T>::getDest() const {
    return this->dest;
}

template<class T>
double Edge<T>::getCapacity() const {
    return this->capacity_;
}

template<class T>
Vertex<T>* Edge<T>::getOrig() const {
    return this->orig;
}

template<class T>
double Edge<T>::getFlow() const {
    return flow_;
}

template<class T>
int Edge<T>::getDomain() const {
    return domain_;
}

template<class T>
void Edge<T>::setFlow(double flow) {
    this->flow_ = flow;
}

template<class T>
void Edge<T>::setCapacity(double capacity) {
    this->capacity_ = capacity;
}

template<class T>
void Edge<T>::setDomain(int domain) {
    this->domain_ = domain;
}

/********************** Graph  ****************************/

template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
std::vector<Vertex<T>*> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T>* Graph<T>::findVertex(const T &in) const {
    for (const auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */

template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(in));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
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
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double c, int domain) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, c, domain);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T>* srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

// Class Constructor that builds the bipartite Graph
template<class T>
Graph<T>::Graph(Data &data) {

    std::vector<Submission> &submissions = data.submissions;
    std::vector<Reviewer> &reviewers = data.reviewers;
    const Parameters &p = data.parameters;
    const Control &c = data.control;

    nodeInfo source = {SOURCE, 0};
    nodeInfo sink = {SINK, -1};

                                /* CREATE THE NODES */
    // Create the source node ID => 0
    this->addVertex(source);
    // Create the sink node ID => -1
    this->addVertex(sink);

    // Create submission nodes
    for (const Submission &s : submissions) {
        this->addVertex({SUBMISSION, s.getId()});
    }
    // Create review nodes R
    for (const Reviewer &r : reviewers) {
        this->addVertex({REVIEWER, r.getId()});
    }
                                /* CREATE THE EDGES*/
    // Create edge from the source node to submission with capacity MinReviewsPerSubmission
    for (const Submission& s : submissions) {
        // When checking the outputs the domain of this edge is always the primary Field
        this->addEdge(source, {SUBMISSION, s.getId()}, p.MinReviewsPerSubmission, s.getPrimaryField());
    }

    // Create edges between submissions and reviewers
    for (const Submission &s : submissions) {
        for (const Reviewer &r : reviewers) {
            nodeInfo submission = {SUBMISSION, s.getId()};
            nodeInfo reviewer = {REVIEWER, r.getId()};

            switch (c.GenerateAssignments) {
                case 0:
                case 1:
                    // Create only primary edges
                    if (s.getPrimaryField() == r.getPrimaryField()) {
                        this->addEdge(submission, reviewer, 1, r.getPrimaryField());
                    }
                    break;

                case 2:
                    // primary + secondary submissions, primary reviewers only
                    if (s.getPrimaryField() == r.getPrimaryField())
                        this->addEdge(submission, reviewer, 1, r.getSecondaryField());

                    // secondary only if both fields exist
                    if (s.getSecondaryField() != 0 && r.getPrimaryField() == s.getSecondaryField())
                        this->addEdge(submission, reviewer, 1, r.getPrimaryField());
                    break;

                case 3:
                    // general case: all primary and secondary
                    if (s.getPrimaryField() == r.getPrimaryField())
                        this->addEdge(submission, reviewer, 1, r.getPrimaryField());

                    if (s.getSecondaryField() != 0 && r.getSecondaryField() != 0 &&
                        s.getSecondaryField() == r.getSecondaryField())
                        this->addEdge(submission, reviewer, 1, r.getSecondaryField());
                    break;
                default: ;
            }
        }
    }

    // Create edges to sink with capacity equal to the MaxReviewsPerReviewer
    for (const Reviewer& r : reviewers) {
        this->addEdge({REVIEWER, r.getId()}, sink, p.MaxReviewsPerReviewer);
    }
}

inline bool operator==(const nodeInfo& n1, const nodeInfo& n2) {
    return n1.id == n2.id && n1.type == n2.type;
}

#endif  /*DA_TP_CLASSES_GRAPH*/
