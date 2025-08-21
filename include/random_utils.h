#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <random>
#include <vector>
#include <cmath>

#include "network_utils.h"

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

vector<vector<int>> initial_population(int population_size, Topology& network) {
    vector<vector<int>> population;
    // Each chromosome should have a size of num_nodes
    // Each chromosome should have atleast one sink node
    vector<int> chrm(network.num_nodes);
    for (int i = 0; i < population_size; ++i) {
        // Randomly select a sink node to be the first node in the chromosome
        unsigned int sink_index = randint(network.num_nodes, network.num_nodes + network.num_sinks - 1);
        unsigned int assign_sink = randint(0, network.num_nodes - 1);
        chrm[assign_sink] = sink_index;
        // Fill the rest of the chromosome with random node indices
        for (size_t j = 0; j < network.num_nodes; ++j) {
            if (j != assign_sink) {
                chrm[j] = randint(0, network.node_list.size() - 1);
            }
        }
        if(!hasCycle(chrm, network)){
            population.push_back(chrm);
        }
        else{
            i--;
        }
    }
    return population;
}


#endif