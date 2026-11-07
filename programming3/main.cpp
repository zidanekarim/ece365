#include "graph.h"
#include <ctime>
#include <fstream>
#include <iostream>

int main() {
    std::string filename;
    std::cout << "Enter name of graph file: ";
    if (!std::getline(std::cin, filename)) return 1;
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Could not open graph file.\n";
        return 1;
    }

    Graph graph;
    std::string source, destination;
    int cost;
    while (input >> source >> destination >> cost) {
        graph.addEdge(source, destination, cost);
    }
    if (source.empty()) {
        std::cerr << "Graph contains no vertices.\n";
        return 1;
    }
    std::string start;
    do {
        std::cout << "Enter name of starting vertex: ";
        if (!std::getline(std::cin, start)) return 1;
    } while (!graph.contains(start));

    std::clock_t begin = std::clock();
    graph.dijkstra(start);
    double seconds = static_cast<double>(std::clock() - begin) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << seconds << '\n';

    std::cout << "Enter name of output file: ";
    if (!std::getline(std::cin, filename)) return 1;
    std::ofstream output(filename, std::ios::binary);
    if (!output) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }
    graph.writePaths(output);
    output.close();
    return output ? 0 : 1;
}
