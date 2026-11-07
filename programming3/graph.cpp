#include "graph.h"
#include "heap.h"
#include <limits>

static constexpr int infinity = std::numeric_limits<int>::max();

Graph::Vertex *Graph::getOrCreate(const std::string &id) {
    auto *vertex = static_cast<Vertex *>(byId.getPointer(id));
    if (!vertex) {
        vertices.push_back({id, {}, infinity, nullptr});
        vertex = &vertices.back();
        byId.insert(id, vertex);
    }
    return vertex;
}

void Graph::addEdge(const std::string &source, const std::string &destination, int cost) {
    Vertex *from = getOrCreate(source);
    Vertex *to = getOrCreate(destination);
    from->outgoing.push_back({to, cost});
}

bool Graph::contains(const std::string &id) {
    return byId.contains(id);
}

void Graph::dijkstra(const std::string &start) {
    heap queue(static_cast<int>(vertices.size()));
    for (Vertex &vertex : vertices) {
        vertex.distance = vertex.id == start ? 0 : infinity;
        vertex.previous = nullptr;
        queue.insert(vertex.id, vertex.distance, &vertex);
    }

    void *payload;
    while (queue.deleteMin(nullptr, nullptr, &payload) == 0) {
        auto *current = static_cast<Vertex *>(payload);
        if (current->distance == infinity) break;
        for (const Edge &edge : current->outgoing) {
            Vertex *next = edge.destination;
            int candidate = current->distance + edge.cost;
            if (candidate < next->distance) {
                next->distance = candidate;
                next->previous = current;
                queue.setKey(next->id, candidate);
            }
        }
    }
}

void Graph::writePaths(std::ostream &out) const {
    std::vector<const Vertex *> path;
    for (const Vertex &vertex : vertices) {
        out << vertex.id << ": ";
        if (vertex.distance == infinity) {
            out << "NO PATH\n";
            continue;
        }
        path.clear();
        for (auto *step = &vertex; step; step = step->previous) {
            path.push_back(step);
        }
        out << vertex.distance << " [";
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            if (it != path.rbegin()) out << ", ";
            out << (*it)->id;
        }
        out << "]\n";
    }
}
