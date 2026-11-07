#ifndef GRAPH_H
#define GRAPH_H

#include "hash.h"
#include <list>
#include <ostream>

class Graph {
    struct Vertex;
    struct Edge {
        Vertex *destination;
        int cost;
    };
    struct Vertex {
        std::string id;
        std::vector<Edge> outgoing;
        int distance;
        Vertex *previous;
    };

    std::list<Vertex> vertices;
    hashTable byId;
    Vertex *getOrCreate(const std::string &id);

public:
    void addEdge(const std::string &source, const std::string &destination, int cost);
    bool contains(const std::string &id);
    void dijkstra(const std::string &start);
    void writePaths(std::ostream &out) const;
};

#endif
