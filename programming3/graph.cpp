#include "graph.h"
#include "heap.h"
#include <limits>

static constexpr int infinity = std::numeric_limits<int>::max();

// getOrCreate - Returns the vertex with the given id, creating it if needed
Graph::Vertex *Graph::getOrCreate(const std::string &id) {
    auto *vertex = static_cast<Vertex *>(byId.getPointer(id));
    if (!vertex) {
        vertices.push_back({id, {}, infinity, nullptr});
        vertex = &vertices.back(); // list keeps vertex pointers valid when more vertices are added
        byId.insert(id, vertex);
    }
    return vertex;
}

// addEdge - Adds a directed edge with the given cost, creating missing vertices
void Graph::addEdge(const std::string &source, const std::string &destination, int cost) {
    Vertex *from = getOrCreate(source);
    Vertex *to = getOrCreate(destination);
    from->outgoing.push_back({to, cost});
}

// contains - Checks whether a vertex with the given id exists
//   true if the vertex exists
//   false otherwise
bool Graph::contains(const std::string &id) {
    return byId.contains(id);
}

// dijkstra - Finds shortest paths from start using the binary heap
// Assumes start exists and edge costs are nonnegative
void Graph::dijkstra(const std::string &start) {
    heap queue(static_cast<int>(vertices.size()));
    // reset distances and previous pointers so we can run from a different start
    for (Vertex &vertex : vertices) {
        vertex.distance = vertex.id == start ? 0 : infinity; // initialize as infinity unless start node
        vertex.previous = nullptr; 
        queue.insert(vertex.id, vertex.distance, &vertex);
    }

    void *payload;
    while (queue.deleteMin(nullptr, nullptr, &payload) == 0) {
        auto *current = static_cast<Vertex *>(payload);
        if (current->distance == infinity) break; // remaining vertices cannot be reached
        for (const Edge &edge : current->outgoing) {
            Vertex *next = edge.destination;
            int candidate = current->distance + edge.cost;
            if (candidate < next->distance) { // found a shorter path through current
                next->distance = candidate;
                next->previous = current;
                queue.setKey(next->id, candidate); // update priority in the heap
            }
        }
    }
}

// writePaths - Writes each vertex's distance and path in insertion order
// Uses results from dijkstra; unreachable vertices are written as NO PATH
void Graph::writePaths(std::ostream &out) const {
    std::vector<const Vertex *> path;
    for (const Vertex &vertex : vertices) {
        out << vertex.id << ": ";
        if (vertex.distance == infinity) {
            out << "NO PATH\n";
            continue;
        }
        path.clear();
        // follow previous pointers back to the starting vertex
        for (auto *step = &vertex; step; step = step->previous) {
            path.push_back(step);
        }
        out << vertex.distance << " [";
        for (auto it = path.rbegin(); it != path.rend(); ++it) { // print from start to destination
            if (it != path.rbegin()) out << ", ";
            out << (*it)->id;
        }
        out << "]\n";
    }
}
