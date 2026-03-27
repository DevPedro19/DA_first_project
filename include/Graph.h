// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include "Data.h"

/**
 * @brief This enum is responsible for representing the type of the node in the graph, which can be a source, sink, submission or reviewer.
 */

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

constexpr int NO_FIELD = 0;

/**
 * @brief This struct is responsible for representing the information of a node in the graph, which consists of its type and id.
 */

struct nodeInfo {
    enum type type;
    int id;
};

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/
/** 
 * @brief This class represents the vertices of the graph, which consist of a nodeInfo and a list of outgoing edges.
 * @param <T> The type of the vertex information, which in this case is nodeInfo.
*/
template <class T>
class Vertex {
public:
    /**
     * @brief Constructor for the Vertex class.
     * @param in The information of the vertex, which consists of its type and id, the struct nodeInfo.
     */
    Vertex(const T &in);

    /**
     * @brief This function returns the information of the vertex, which consists of its type and id, the struct nodeInfo.
     * @return The information of the vertex, which consists of its type and id, the
     */
    T getInfo() const;

    /**
     * @brief This function returns the list of outgoing edges of the vertex.
     * @return The list of outgoing edges of the vertex.
     */
    std::vector<Edge<T> *> getAdj() const;

    /**
     * @brief This function returns whether the vertex has been visited or not, which will be used for the algorithm to find a paths between the source and the sink.
     * @return Whether the vertex has been visited or not.
     */
    bool isVisited() const;

    /**
     * @brief This function returns the previous edge through which the vertex was reached, which will be used for the algorithm to find a paths between the source and the sink.
     * @return The previous edge, which will be used to find the path between the source and the sink.
     */
    Edge<T>* getPath() const;

    /**
     * @brief This function returns the list of incoming edges of the vertex.
     * It's used to find if a edge is a forward or backward edge when finding the augmenting path and to update the flow of the edges accordingly.
     * @return The list of incoming edges of the vertex.
     */
    std::vector<Edge<T>*> getIncoming() const;

    /**
     * @brief This function sets the visited field of the vertex.
     * @param visited Whether the vertex has been visited or not.
     */
    void setVisited(bool visited);

    /**
     * @brief This function sets the information of the vertex, which consists of its type and id, the struct nodeInfo.
     * @param info The information of the vertex, which consists of its type and id, the struct nodeInfo.
     */
    void setInfo(T info);

    /**
     * @brief This function sets the previous edge through which the vertex was reached, which will be used for the algorithm to find a paths between the source and the sink.
     * @param path The previous edge.
     */
    void setPath(Edge<T> *path);

    /**
     * @brief This function adds an outgoing edge to the vertex, with a given destination vertex and edge weight.
     * @param d The destination vertex of the edge.
     * @param capacity The weight of the edge, which in this case is the capacity of the edge.
     * @param domain The domain of the edge, which is used to check if the edge is a primary or secondary edge when generating the assignments.
     * @details The edge is added to the list of outgoing edges of the vertex, and a corresponding incoming edge is added to the destination vertex.
     */
    Edge<T>* addEdge(Vertex<T> *d, double capacity, int domain = 0);

    /**
     * @brief This function removes an outgoing edge of the vertex, given the id of the destination vertex.
     * @param id The id of the destination vertex of the edge to be removed.
     * @return Whether the edge was successfully removed or not.
     */
    bool removeEdge(const T &id);

    /**
     * @brief This function removes all outgoing edges of the vertex.
     * It's used to remove all edges of a vertex when that vertex is removed from the graph.
     */
    void removeOutgoingEdges();

protected:

    /**
     * @brief The infomation of the vertex, nodeInfo
     */
    T info_;          

    /**
     * @brief The list of outgoing edges of the vertex.
     */
    std::vector<Edge<T>*> adj;

    // auxiliary fields

    /**
     * @brief Boolean value indicating whether the vertex has been visited or not.
     */
    bool visited_ = false;

    /**
     * @brief Pointer to the previous edge through which the vertex was reached.
     */
    Edge<T> *path_ = nullptr;


    /**
     * @brief The list of incoming edges of the vertex.
     */
    std::vector<Edge<T>*> incoming;


    /**
     * @brief Auxiliary function to remove an outgoing edge of a vertex.
     * @param edge The edge to be removed.
     */
    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

/**
 * @brief This class represents the edges of the graph, which consist of a destination vertex, capacity, flow and domain.
 * @param <T> The type of the vertex information, which in this case is nodeInfo.
 */

template <class T>
class Edge {
public:

    /**
     * @brief Constructor for the Edge class.
     * @param orig The origin vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param capacity The capacity of the edge, which is the weight of the edge.
     * @param domain The domain of the edge, which is used to check if the edge is a primary or secondary edge when generating the assignments.
     */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double capacity, int domain=0);

    /**
     * @brief This function returns the destination vertex of the edge.
     * @return The destination vertex of the edge.
     */
    Vertex<T>* getDest() const;

    /**
     * @brief This function returns the capacity of the edge.
     * @return The capacity of the edge.
     */
    double getCapacity() const;

    /**
     * @brief This function returns the origin vertex of the edge.
     * @return The origin vertex of the edge.
     */
    Vertex<T>* getOrig() const;

    /**
     * @brief This function returns the current flow of the edge.
     * @return The current flow of the edge.
     */
    double getFlow() const;

    /**
     * @brief This function returns the domain of the edge, which is used to check if the edge is a primary or secondary edge when generating the assignments.
     * @return The domain of the edge.
     */
    int getDomain() const;

    /**
     * @brief This function sets the current flow of the edge.
     * @param flow The current flow of the edge, which is updated when an augmenting path is found in the max flow algorithm.
     */
    void setFlow(double flow);

    /**
     * @brief This function sets the domain of the edge.
     * @param domain The domain of the edge.
     */
    void setDomain(int domain);

    /**
     * @brief This function sets the capacity of the edge.
     * @param capacity The capacity of the edge, which is the weight of the edge.
     */
    void setCapacity(double capacity);

protected:

    /**
     * @brief The destination vertex of the edge.
     */
    Vertex<T>*dest; 

    /**
     * @brief The capacity of the edge.
     */
    double capacity_;

    /**
     * @brief The domain of the edge.
     */
    int domain_;

    /**
     * @brief The origin vertex of the edge.
     * @details This field is used for bidirectional edges, which are created in the residual graph when running the max flow algorithm. In the original graph, the edges are directed from the source to the sink, so this field is not used and can be set to nullptr.
     */
    Vertex<T> *orig;

    /**
     * @brief The current flow of the edge
     * @details Initially, the flow of the edge is set to 0, and it is updated when an augmenting path is found in the max flow algorithm. The flow of an edge cannot exceed its capacity, and it cannot be negative.
     */
    double flow_ = 0; // current flow of the edge

};

/********************** Graph  ****************************/


/**
 * @brief This class represents the graph, which consists of a set of vertices and edges. It also contains the constructor that builds the bipartite graph from the input data and auxiliary functions to add and remove vertices and edges, as well as to find a vertex with a given content.
 * @param <T> The type of the vertex information, which in this case is nodeInfo.
 */
template <class T>
class Graph {
public:

    /**
     * @brief Constructor for the Graph class, which builds the bipartite graph from the input data.
     * @param data The input data, which consists of the list of submissions, reviewers, parameters and control. The graph is built according to the specifications of the input data, creating the vertices and edges accordingly.
     * @details The graph is built as follows: the source node is connected to each submission node with an edge of capacity equal to the minimum number of reviews per submission, and the domain of the edge is the primary field of the submission. Each submission node is connected to each reviewer node with an edge of capacity 1, and the domain of the edge is determined by the GenerateAssignments parameter in the control struct. Finally, each reviewer node is connected to the sink node with an edge of capacity equal to the maximum number of reviews per reviewer.
     */
    Graph(Data &data);
   
    /**
      * @brief Auxiliary function to find a vertex with a given content.
      * @param in The content of the vertex to be found, which consists of its type and id, the struct nodeInfo.
      * @return A pointer to the vertex with the given content, or nullptr if such vertex does not exist in the graph.
     */
    Vertex<T>* findVertex(const T &in) const; //Auxiliary function to find a vertex with a given the content.

     /**
      * @brief This function adds a vertex with a given content.
      * @param in The content of the vertex to be added, which consists of its type and id, the struct nodeInfo.
      * @return Whether the vertex was successfully added or not. The vertex is not added if
      * @details The vertex is not added if a vertex with that content already exists in the graph. The content of the vertex consists of its type and id, the struct nodeInfo, so if there are multiple vertices with the same type and id, only one of them will be added to the graph.
      */
    bool addVertex(const T &in);

    /**
     * @brief This function removes a vertex with a given content, and all outgoing and incoming edges of that vertex.
     * @param in The content of the vertex to be removed, which consists of its type and id, the struct nodeInfo.
     * @return Whether the vertex was successfully removed or not. The vertex is not removed if
     * @details The vertex is not removed if such vertex does not exist in the graph. When a vertex is removed, all outgoing edges of that vertex are removed, and for each outgoing edge, the corresponding incoming edge in the destination vertex is also removed. Additionally, for each incoming edge of the vertex to be removed, the corresponding outgoing edge in the origin vertex is also removed.
     */
    bool removeVertex(const T &in);

    /**
     * @brief This function adds an edge to the graph.
     * @param sourc The content of the source vertex of the edge, which consists of its type and id, the struct nodeInfo.
     * @param dest The content of the destination vertex of the edge, which consists of its type and id, the struct nodeInfo.
     * @param c The capacity of the edge, which is the weight of the edge.
     * @param domain The domain of the edge, which is used to check if the edge is a primary or secondary edge when generating the assignments.
     * @return Whether the edge was successfully added or not. The edge is not added if
     * @details The edge is not added if the source or destination vertex does not exist in the graph. The edge is identified by the source and destination vertices, so if there are multiple edges between the same pair of vertices, all of them will be added.
     */
    bool addEdge(const T &sourc, const T &dest, double c, int domain = 0);

    /**
     * @brief This function removes an edge from the graph.
     * @param sourc The content of the source vertex of the edge, which consists of its type and id, the struct nodeInfo.
     * @param dest The content of the destination vertex of the edge, which consists of its type and id, the struct nodeInfo.
     * @return Whether the edge was successfully removed or not
     * @details The edge is not removed if such edge does not exist in the graph. The edge is identified by the source and destination vertices, so if there are multiple edges between the same pair of vertices, all of them will be removed.
     */
    bool removeEdge(const T &source, const T &dest);


    /**
      * @brief This function returns the number of vertices in the graph.
      * @return The number of vertices in the graph.
     */
    int getNumVertex() const;


    /**
     * @brief This function returns the list of vertices in the graph.
     * @return The list of vertices in the graph.
     */
    std::vector<Vertex<T>*> getVertexSet() const;

protected:

    /**
     * @brief The list of vertices in the graph.
     * @details The graph is represented as an adjacency list, where each vertex has a list of outgoing edges. The list of vertices in the graph is stored in the vertexSet vector, and each vertex has a list of outgoing edges stored in the adj vector. The graph can have multiple edges between the same pair of vertices, so the adj vector can contain multiple edges with the same destination vertex.
     */
    std::vector<Vertex<T>*> vertexSet;
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

    // Considerando submision para reviewer
    bool primaryPrimary = (data.parameters.PrimarySubmissionDomain && data.parameters.PrimaryReviewerExpertise);
    bool primarySecondary = (data.parameters.PrimarySubmissionDomain && data.parameters.SecondaryReviewerExpertise);
    bool secondaryPrimary = (data.parameters.SecondarySubmissionDomain && data.parameters.PrimaryReviewerExpertise);
    bool secondarySecondary = (data.parameters.SecondarySubmissionDomain && data.parameters.SecondaryReviewerExpertise);


    // Create edges between submissions and reviewers
    for (const Submission &s : submissions) {
        for (const Reviewer &r : reviewers) {
            nodeInfo submission = {SUBMISSION, s.getId()};
            nodeInfo reviewer = {REVIEWER, r.getId()};

            // helper lambda function that reduces code repetition
            auto add = [&](int field) {
                this->addEdge(submission, reviewer, 1, field);
            };

            switch (c.GenerateAssignments) {
                case 0:
                case 1:
                    // Create only primary edges
                    if (primaryPrimary &&
                        s.getPrimaryField() == r.getPrimaryField()) {
                        add(r.getPrimaryField());
                    }
                    break;

                case 2:
                    // primary + secondary submissions, primary reviewers only
                    if (primaryPrimary &&
                        s.getPrimaryField() == r.getPrimaryField())
                       add(r.getPrimaryField());

                    // secondary only if both fields exist
                    if (secondaryPrimary &&
                        s.getSecondaryField() != NO_FIELD &&
                        s.getSecondaryField() == r.getPrimaryField())
                        add(r.getPrimaryField());
                    break;

                case 3:
                    // general case: all primary and secondary

                    // primary only if both fields exist
                    if (primaryPrimary &&
                        s.getPrimaryField() == r.getPrimaryField())
                        add(r.getPrimaryField());

                    // primary with secondary only if both fields exist
                    if (primarySecondary &&
                        r.getSecondaryField() != NO_FIELD &&
                        s.getPrimaryField() == r.getSecondaryField())
                        add(r.getSecondaryField());

                    // secondary with primary only if both fields exist
                    if (secondaryPrimary &&
                        s.getSecondaryField() != NO_FIELD &&
                        s.getSecondaryField() == r.getPrimaryField())
                        add(r.getPrimaryField());

                    // secondary with secondary only if both fields exist
                    if (secondarySecondary &&
                        s.getSecondaryField() != NO_FIELD &&
                        r.getSecondaryField() != NO_FIELD &&
                        s.getSecondaryField() == r.getSecondaryField())
                        add(r.getSecondaryField());
                    break;
                default: ;
            }
        }
    }

    // Create edges to sink with capacity equal to the MaxReviewsPerReviewer
    for (const Reviewer& r : reviewers) {
        this->addEdge({REVIEWER, r.getId()}, sink, p.MaxReviewsPerReviewer);
    }

    for (auto v: vertexSet) {
        for (auto e : v->getAdj()) {
            std::cout << enumToString(v->getInfo().type) << " " << v->getInfo().id << " -- "
                      << e->getCapacity() << " --> " << enumToString(e->getDest()->getInfo().type)
                      << " " << e->getDest()->getInfo().id << std::endl;
        }
    }
}

/**
 * @brief Equality operator for nodeInfo struct, which is used to compare the content of the vertices in the graph. Two nodeInfo are considered equal if they have the same type and id.
 * @param n1 The first nodeInfo to be compared.
 * param n2 The second nodeInfo to be compared.
 * @return Whether the two nodeInfo are equal or not.
 */

inline bool operator==(const nodeInfo& n1, const nodeInfo& n2) {
    return n1.id == n2.id && n1.type == n2.type;
}

#endif  /*DA_TP_CLASSES_GRAPH*/
