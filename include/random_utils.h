#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <random>
#include <vector>
#include <cmath>

#include "network_utils.h"
#include "debug_utils.h"

random_device rd;  // Seed the random number engine
mt19937 gen(rd()); // Standard mersenne_twister_engine

int randint(int low, int high) {
    if (high < low) {
        throw std::invalid_argument("randint called with invalid range: [" +
            std::to_string(low) + ", " + std::to_string(high) + "]");
    }
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

double uniform(double low, double high) {
    uniform_real_distribution<> dist(low, high);
    return dist(gen);
}

vector<vector<int>> initial_population(SimulationData& simulator, Topology& network) {
    int population_size = simulator.population_size;
    vector<vector<int>> population;
    // Each chromosome should have a size of num_nodes
    // Each chromosome should have atleast one sink node
    vector<int> chrm(network.num_nodes);
    for (int i = 0; i < population_size; ++i) {
        // Randomly select a sink node to be the first node in the chromosome
        unsigned int sink_index;
        vector<int> incoming;
        while(incoming.empty()){
            sink_index = randint(network.num_nodes, network.num_nodes + network.num_sinks - 1);
            incoming = network.get_incoming_nodes(sink_index);
        }

        int incom_index = randint(0,incoming.size() - 1);
        unsigned int assign_sink = incoming[incom_index];
        chrm[assign_sink] = sink_index;
        // Fill the rest of the chromosome with random node indices
        for (size_t j = 0; j < network.num_nodes; ++j) {
            if (j != assign_sink) {
                vector<int> reachable = network.get_reachable_nodes(j);
                int reach_index = randint(0, reachable.size() - 1);
                chrm[j] = reachable[reach_index];
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

string random_string(size_t length) {
    const string CHARACTERS = "0123456789abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<> dist(0, CHARACTERS.size() - 1);
    string result;
    for (size_t i = 0; i < length; ++i) {
        result += CHARACTERS[dist(gen)];
    }
    return result;
}


#endif