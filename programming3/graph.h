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
    // getOrCreate - Returns the vertex with the given id, creating it if needed
    Vertex *getOrCreate(const std::string &id);

public:
    // addEdge - Adds a directed edge with the given cost, creating missing vertices
    void addEdge(const std::string &source, const std::string &destination, int cost);

    // contains - Checks whether a vertex with the given id exists
    // Returns:
    //   true if the vertex exists
    //   false otherwise
    bool contains(const std::string &id);

    // dijkstra - Finds shortest paths from start and stores distances and previous pointers
    // Assumes start exists and edge costs are nonnegative (otherwise cycles)
    void dijkstra(const std::string &start);

    // writePaths - Writes each vertex's distance and path in insertion order
    // Uses results from dijkstra; unreachable vertices are written as NO PATH
    void writePaths(std::ostream &out) const;
};

#endif
