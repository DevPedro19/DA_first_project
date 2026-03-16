// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <vector>
#include <queue>

#include "Data.h"

class Edge;

#define INF std::numeric_limits<double>::max()

// Offset to avoid reviewers and submissions with the same id

typedef std::string string;

/************************* Vertex  **************************/

class Vertex {
public:
    Vertex(string name);

    string getName() const;
    std::vector<Edge*> getAdj() const;
    bool isVisited() const;
    Edge* getPath() const;
    std::vector<Edge*> getIncoming() const;

    void setVisited(bool visited);

    void setName(const string &name);
    void setPath(Edge *path);
    Edge* addEdge(Vertex *d, double capacity);
    bool removeEdge(string name);
    void removeOutgoingEdges();

protected:
    std::string name_;                // name of the node
    std::vector<Edge*> adj;  // outgoing edges

    // auxiliary fields
    bool visited_ = false; // used to mark if a node has been visited
    Edge* path_ = nullptr; // used to get the path between two nodes

    std::vector<Edge*> incoming; // incoming edges

    void deleteEdge(Edge *edge);
};

/********************** Edge  ****************************/

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double capacity);

    Vertex* getDest() const;
    double getCapacity() const;
    Vertex* getOrig() const;
    double getFlow() const;

    void setFlow(double flow);
    void setCapacity(double capacity);

protected:
    Vertex* dest; // destination vertex
    double capacity_; // max capacity of the edge

    // used for bidirectional edges
    Vertex* orig;

    double flow_ = 0; // current flow of the edge

};

/********************** Graph  ****************************/

class Graph {
public:
    Graph(Data &data);
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex* findVertex(const string &name) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const string &name);
    bool removeVertex(const string &name);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const string &sourc, const string &dest, double c);
    bool removeEdge(const string &source, const string &dest);

    int getNumVertex() const;

    std::vector<Vertex*> getVertexSet() const;


protected:
    std::vector<Vertex*> vertexSet;    // vertex set

};
#endif  /*DA_TP_CLASSES_GRAPH*/
