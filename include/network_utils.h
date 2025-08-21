#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "types.h"

// Add function to check for cycles in a chromosome
bool hasCycle(const vector<int>& chromosome, const Topology& network) {
    // Implement cycle detection logic here
    // This could be done using a depth-first search (DFS) or union-find algorithm
    // For now, we will return false as a placeholder
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