#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
#include <climits>
#include <chrono>
#include <omp.h>
#include <mpi.h>

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
    #pragma omp parallel for reduction(+:total_demand)
    for (size_t i = 0; i < nodes.size(); ++i) {
        total_demand += get<1>(nodes[i]);
    }
    return total_demand;
}

void findBestRoute(const vector<tuple<vector<int>, int>>& solutions, vector<int>& best_route, int& best_cost) {
    best_cost = INT_MAX;
    #pragma omp parallel for
    for (size_t i = 0; i < solutions.size(); ++i) {
        const vector<int>& route = get<0>(solutions[i]);
        int solution_cost = get<1>(solutions[i]);
        #pragma omp critical
        {
            if (solution_cost < best_cost) {
                best_route = route;
                best_cost = solution_cost;
            }
        }
    }
}

void findRoutes(int num_nodes, vector<tuple<int, int, int>>& nodes, int num_edges, map<int, vector<pair<int, int>>>& graph, int current_cost, int current_node, int current_supply, int visited_cities_count, vector<int> visited_cities, vector<tuple<vector<int>, int>>& solutions) {
    #pragma omp parallel for
    for (size_t i = 0; i < graph[current_node].size(); ++i) {
        int next_node = graph[current_node][i].first;
        int edge_weight = graph[current_node][i].second;

        if (next_node != 0 && (visited_cities_count + 1 >= 3 || current_supply - get<1>(nodes[next_node]) <= 0)) {
            continue;
        }

        vector<int> new_visited_cities = visited_cities;
        new_visited_cities.push_back(get<0>(nodes[next_node]));

        if (next_node == 0) {
            if (calculateTotalDemand(nodes) == 0) {
                #pragma omp critical
                solutions.emplace_back(new_visited_cities, current_cost + edge_weight);
                continue;
            }
            findRoutes(num_nodes, nodes, num_edges, graph, current_cost + edge_weight, next_node, 15, 0, new_visited_cities, solutions);
        } else {
            if (get<1>(nodes[next_node]) != 0 && !contains(visited_cities, get<0>(nodes[next_node]))) {
                vector<tuple<int, int, int>> updated_nodes = nodes;
                get<1>(updated_nodes[next_node]) = 0;

                findRoutes(num_nodes, updated_nodes, num_edges, graph, current_cost + edge_weight, next_node, current_supply - get<1>(nodes[next_node]), visited_cities_count + 1, new_visited_cities, solutions);
            }
        }
    }
}

void displayBestRoute(const vector<int>& best_route, int best_cost) {
    cout << "Melhor rota: ";
    for (int city : best_route) {
        cout << city << " ";
    }
    cout << "\nCusto: " << best_cost << endl;
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        cerr << "O programa requer pelo menos 2 processos MPI" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    string filename = argv[1];

    int num_nodes;
    vector<tuple<int, int, int>> nodes;
    int num_edges;
    map<int, vector<pair<int, int>>> graph;

    readGraphFromFile(filename, num_nodes, nodes, num_edges, graph);

    int initial_supply = 15;
    int initial_cost = 0, start_node = 0, initial_cities_visited = 0;
    vector<int> initial_visited_cities = {0};
    vector<tuple<vector<int>, int>> potential_solutions;

    auto start_time = high_resolution_clock::now();

    if (rank == 0) {
        findRoutes(num_nodes, nodes, num_edges, graph, initial_cost, start_node, initial_supply, initial_cities_visited, initial_visited_cities, potential_solutions);
    }

    if (rank == 0) {
        vector<int> best_route;
        int minimal_cost;
        findBestRoute(potential_solutions, best_route, minimal_cost);

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time);

        displayBestRoute(best_route, minimal_cost);
        cout << "Tempo de busca de rotas: " << duration.count() << " ms" << endl;
    }

    MPI_Finalize();

    return 0;
}
