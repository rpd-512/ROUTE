#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <random>
#include <vector>
#include <cmath>

random_device rd;  // Seed the random number engine
mt19937 gen(rd()); // Standard mersenne_twister_engine

int randint(int low, int high) {
    uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

double uniform(double low, double high) {
    uniform_real_distribution<> dist(low, high);
    return dist(gen);
}

vector<vector<int>> initial_population(int size, Topology& nodes) {
    vector<int> sink_ids;
    for (const auto& node : nodes) {
        if (node.isSink) {
            sink_ids.push_back(node.id);
        }
    }
    if (sink_ids.empty()) {
        throw runtime_error("No sink nodes found in the topology.");
    }

    vector<vector<int>> population;
    for (int i = 0; i < size; i++) {
        vector<int> chrm(nodes.size());

        // Ensure at least one sink is present
        size_t sink_pos = randint(0, nodes.size() - 1);
        chrm[sink_pos] = sink_ids[randint(0, sink_ids.size() - 1)];

        // Fill the rest randomly
        for (size_t j = 0; j < nodes.size(); j++) {
            if (j == sink_pos) continue; // already assigned sink
            chrm[j] = randint(0, nodes.size() - 1);
        }

        population.push_back(chrm);
    }
    return population;
}


#endif