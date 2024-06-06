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
    return value != 0 && find(vec.begin(), vec.end(), value) != vec.end();
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

void randomSwap(vector<int>& route) {
    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<int> distribution(1, route.size() - 2);
    int i = distribution(gen);
    int j = distribution(gen);

    swap(route[i], route[j]);
}

int calculateRouteCost(const vector<int>& route, const map<int, vector<pair<int, int>>>& graph) {
    int cost = 0;
    for (int i = 0; i < route.size() - 1; ++i) {
        int from = route[i];
        int to = route[i + 1];
        for (const auto& edge : graph.at(from)) {
            if (edge.first == to) {
                cost += edge.second;
                break;
            }
        }
    }
    return cost;
}

void localSearch(vector<int>& route, int& cost, const map<int, vector<pair<int, int>>>& graph) {
    int iteration = 0;
    int max_iterations = 1000; // Limite máximo de iterações

    while (iteration < max_iterations) {
        vector<int> new_route = route;
        randomSwap(new_route);
        int new_cost = calculateRouteCost(new_route, graph);
        if (new_cost < cost) {
            route = new_route;
            cost = new_cost;
            iteration = 0; // Reinicia as iterações se houve melhoria
        } else {
            iteration++;
        }
    }
}

void displayRoute(const vector<int>& route, int cost) {
    cout << "Rota: ";
    for (int city : route) {
        cout << city << " ";
    }
    cout << "\nCusto: " << cost << endl;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << endl;
        return 1;
    }

    string filename = argv[1];

    int num_nodes;
    vector<tuple<int, int, int>> nodes;
    int num_edges;
    map<int, vector<pair<int, int>>> graph;

    readGraphFromFile(filename, num_nodes, nodes, num_edges, graph);

    // Gerar uma rota inicial aleatória
    vector<int> initial_route;
    for (int i = 1; i < num_nodes; ++i) {
        initial_route.push_back(i);
    }
    random_shuffle(initial_route.begin(), initial_route.end());

    initial_route.insert(initial_route.begin(), 0); // Adiciona o depósito no início e fim da rota
    initial_route.push_back(0);

    int initial_cost = calculateRouteCost(initial_route, graph);

    auto start_time = high_resolution_clock::now();

    // Aplica a busca local na rota inicial
    localSearch(initial_route, initial_cost, graph);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    displayRoute(initial_route, initial_cost);
    cout << "Tempo de busca local: " << duration.count() << " ms" << endl;

    return 0;
}
