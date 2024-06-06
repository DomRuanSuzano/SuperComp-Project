#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

void readGraphFromFile(const string& filename, int& num_nodes, vector<pair<int, int>>& nodes, int& num_edges, vector<vector<int>>& graph) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return;
    }

    file >> num_nodes;
    nodes.resize(num_nodes);
    for (int i = 0; i < num_nodes; ++i) {
        int x, y;
        file >> x >> y;
        nodes[i] = make_pair(x, y);
    }

    file >> num_edges;
    graph.assign(num_nodes, vector<int>(num_nodes, 0));
    for (int i = 0; i < num_edges; ++i) {
        int from, to, weight;
        file >> from >> to >> weight;
        graph[from][to] = weight;
        graph[to][from] = weight; // assuming the graph is undirected
    }
}

int nearestNeighbor(int start, const vector<vector<int>>& graph, vector<bool>& visited) {
    int nearest = -1;
    int minDistance = INT_MAX;
    for (int i = 0; i < graph[start].size(); ++i) { // Corrigindo o laço para iterar apenas sobre os vizinhos de start
        if (!visited[i] && graph[start][i] != 0 && graph[start][i] < minDistance) {
            minDistance = graph[start][i];
            nearest = i;
        }
    }
    return nearest;
}


vector<int> nearestNeighborHeuristic(const vector<vector<int>>& graph) {
    int num_nodes = graph.size();
    vector<bool> visited(num_nodes, false);
    vector<int> route;
    int current_city = 0; // start from city 0
    visited[current_city] = true;
    route.push_back(current_city);
    for (int i = 1; i < num_nodes; ++i) {
        int next_city = nearestNeighbor(current_city, graph, visited);
        if (next_city != -1) {
            visited[next_city] = true;
            route.push_back(next_city);
            current_city = next_city;
        } else {
            cerr << "Error: Invalid next city found." << endl;
            return {}; // return empty vector in case of error
        }
    }
    route.push_back(0); // return to starting city
    return route;
}

void displayRoute(const vector<int>& route) {
    cout << "Best route: ";
    for (int city : route) {
        cout << city << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];

    int num_nodes;
    vector<pair<int, int>> nodes;
    int num_edges;
    vector<vector<int>> graph;

    readGraphFromFile(filename, num_nodes, nodes, num_edges, graph);

    auto start_time = high_resolution_clock::now();
    vector<int> route = nearestNeighborHeuristic(graph);
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    displayRoute(route);
    cout << "Execution time: " << duration.count() << " ms" << endl;

    return 0;
}
