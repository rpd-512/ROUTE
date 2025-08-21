#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "types.h"

bool hasCycle(const vector<int>& chromosome, const Topology& network) {
    int n = network.num_nodes;

    for (int start = 0; start < n; ++start) {
        vector<bool> visited(n, false);
        int current = start;

        while (true) {
            if (current < 0 || current >= n) break;  // invalid hop
            if (network.node_list[current].isSink) break; // reached sink, no cycle
            if (visited[current]) return true; // cycle detected

            visited[current] = true;
            current = chromosome[current]; // follow next hop
        }
    }
    return false;
}

// Function to calculate latency
double calculateLatency(const vector<int>& chromosome, const Topology& network) {
    double latency = 0.0;
    // Implement latency calculation based on the chromosome and network topology
    return latency;
}
// Function to calculate energy usage
double calculateEnergyUsage(const vector<int>& chromosome, const Topology& network) {
    double energyUsage = 0.0;
    // Implement energy usage calculation based on the chromosome and network topology
    return energyUsage;
}
// Function to calculate throughput
double calculateThroughput(const vector<int>& chromosome, const Topology& network) {
    double throughput = 0.0;
    // Implement throughput calculation based on the chromosome and network topology
    return throughput;
}
// Function to calculate fitness
double calculateFitness(const vector<int>& chromosome, const Topology& network) {
    double fitness = 0.0;
    // Combine latency, energy usage, and throughput into a single fitness score
    return fitness;
}

#endif