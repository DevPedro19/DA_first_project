//
// Created by Gabriel Magalhaes on 16/03/2026.
//
#include "Graph.h"

/************************* Vertex  **************************/

Vertex::Vertex(string name): name_(name) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
Edge* Vertex::addEdge(Vertex *d, double capacity) {
    auto newEdge = new Edge(this, d, capacity);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex::removeEdge(string name) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getName() == name) {
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
void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

string Vertex::getName() const {
    return this->name_;
}

std::vector<Edge*> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited_;
}

Edge* Vertex::getPath() const {
    return this->path_;
}

std::vector<Edge*> Vertex::getIncoming() const {
    return this->incoming;
}

void Vertex::setVisited(const bool visited) {
    this->visited_ = visited;
}

void Vertex::setName(const string &name) {
    this->name_ = name;
}

void Vertex::setPath(Edge *path) {
    this->path_ = path;
}

void Vertex::deleteEdge(Edge *edge) {
    Vertex* dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getName() == name_) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double capacity) : dest(dest), capacity_(capacity), orig(orig) {}

Vertex* Edge::getDest() const {
    return this->dest;
}

double Edge::getCapacity() const {
    return this->capacity_;
}

Vertex* Edge::getOrig() const {
    return this->orig;
}

double Edge::getFlow() const {
    return flow_;
}

void Edge::setFlow(double flow) {
    this->flow_ = flow;
}

void Edge::setCapacity(double capacity) {
    this->capacity_ = capacity;
}

/********************** Graph  ****************************/

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex*> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex* Graph::findVertex(const string &name) const {
    for (const auto v : vertexSet)
        if (v->getName() == name)
            return v;
    return nullptr;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const string &name) {
    if (findVertex(name) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(name));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
bool Graph::removeVertex(const string &name) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getName() == name) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getName());
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
bool Graph::addEdge(const string &sourc, const string &dest, double c) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, c);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
bool Graph::removeEdge(const string &sourc, const string &dest) {
    Vertex* srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

// Class Constructor that builds the bipartite Graph
Graph::Graph(Data &data) {

    std::vector<Submission> &submissions = data.submissions;
    std::vector<Reviewer> &reviewers = data.reviewers;
    const Parameters &p = data.parameters;
    const Control &c = data.control;

                                /* CREATE THE NODES */
    // Create the source node ID => 0
    this->addVertex("Source");
    // Create the sink node ID => -1
    this->addVertex("Sink");

    // Create submission nodes
    for (const Submission &s : submissions) {
        string id = "Submission " + std::to_string(s.getId());
        this->addVertex(id);
    }
    // Create review nodes R
    for (const Reviewer &r : reviewers) {
        string id = "Reviewer " + std::to_string(r.getId());
        this->addVertex(id);
    }
                                /* CREATE THE EDGES*/
    // Create edge from the source node to submission with capacity MinReviewsPerSubmission
    for (const Submission& s : submissions) {
        string id = "Submission " + std::to_string(s.getId());
        this->addEdge("Source", id, p.MinReviewsPerSubmission);
    }

    // Create edges between submisions and reviewers
    for (const Submission &s : submissions) {
        for (const Reviewer &r : reviewers) {
            string s_id = "Submission " + std::to_string(s.getId());
            string r_id = "Reviewer " + std::to_string(r.getId());

            switch (c.GenerateAssignments) {
                case 0:
                case 1:
                    // Create only primary edges
                    if (s.getPrimaryField() == r.getPrimaryField()) {
                        this->addEdge(s_id, r_id, 1);
                    }
                    break;

                case 2:
                    // primary + secondary submissions, primary reviewers only
                    if (s.getPrimaryField() == r.getPrimaryField())
                        this->addEdge(s_id, r_id, 1);

                    // secondary only if both fields exist
                    if (s.getSecondaryField() != 0 && r.getPrimaryField() == s.getSecondaryField())
                        this->addEdge(s_id, r_id, 1);
                    break;

                case 3:
                    // general case: all primary and secondary
                    if (s.getPrimaryField() == r.getPrimaryField())
                        this->addEdge(s_id, r_id, 1);

                    if (s.getSecondaryField() != 0 && r.getSecondaryField() != 0 &&
                        s.getSecondaryField() == r.getSecondaryField())
                        this->addEdge(s_id, r_id, 1);
                    break;
                default: ;
            }
        }
    }

    // Create edges to sink with capacity equal to the MaxReviewsPerReviewer
    for (const Reviewer& r : reviewers) {
        string id = "Reviewer " + std::to_string(r.getId());
        this->addEdge(id, "Sink", p.MaxReviewsPerReviewer);
    }
}
