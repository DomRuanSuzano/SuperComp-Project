#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
#include <climits>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

bool contains(const vector<int>& vec, int value) {
    return find(vec.begin(), vec.end(), value) != vec.end();
}

void readGraphFromFile(const string& filename, int& num_nodes, vector<tuple<int, int, int>>& nodes, int& num_edges, map<int, vector<pair<int, int>>>& graph) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return;
    }

    file >> num_nodes;
    nodes.resize(num_nodes);
    for (int i = 1; i < num_nodes; ++i) {
        int city, demand;
        file >> city >> demand;
        nodes[i] = make_tuple(city, demand, 0);
    }

    file >> num_edges;
    for (int i = 0; i < num_edges; ++i) {
        int from, to, weight;
        file >> from >> to >> weight;
        graph[from].emplace_back(to, weight);
    }
}

int calculateTotalDemand(const vector<tuple<int, int, int>>& nodes) {
    int total_demand = 0;
    for (const auto& node : nodes) {
        total_demand += get<1>(node);
    }
    return total_demand;
}

int calculateDistance(int city1, int city2, const map<int, vector<pair<int, int>>>& graph) {
    auto it = find_if(graph.at(city1).begin(), graph.at(city1).end(),
                      [&](const pair<int, int>& p) { return p.first == city2; });
    if (it != graph.at(city1).end()) {
        return it->second;
    }
    return INT_MAX;
}

int calculateRouteCost(const vector<int>& route, const map<int, vector<pair<int, int>>>& graph) {
    int cost = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        int city1 = route[i];
        int city2 = route[i + 1];
        cost += calculateDistance(city1, city2, graph);
    }
    return cost;
}

void localSearch(vector<int>& route, const map<int, vector<pair<int, int>>>& graph) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, route.size() - 2); // Exclude start and end nodes

    int num_iterations = 1000;
    while (num_iterations-- > 0) {
        int i = distrib(gen);
        int j = distrib(gen);
        if (i == j) continue;

        vector<int> new_route = route;
        swap(new_route[i], new_route[j]);
        
        int new_cost = calculateRouteCost(new_route, graph);
        int old_cost = calculateRouteCost(route, graph);

        if (new_cost < old_cost) {
            route = new_route;
        }
    }
}

void displayRoute(const vector<int>& route, int cost) {
    cout << "Melhor rota: ";
    for (int city : route) {
        cout << city << " ";
    }
    cout << "\nCusto: " << cost << endl;
}

int main() {
    int num_nodes;
    vector<tuple<int, int, int>> nodes;
    int num_edges;
    map<int, vector<pair<int, int>>> graph;

    readGraphFromFile("grafo.txt", num_nodes, nodes, num_edges, graph);

    auto start_time = high_resolution_clock::now();
    vector<int> route(num_nodes);
    iota(route.begin(), route.end(), 0); // Initialize route with all nodes in sequence
    localSearch(route, graph);
    int cost = calculateRouteCost(route, graph);
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    displayRoute(route, cost);
    cout << "Tempo de execução: " << duration.count() << " milissegundos" << endl;

    return 0;
}
