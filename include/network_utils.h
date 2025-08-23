#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "types.h"
#include <cfloat>
#include "debug_utils.h"
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
 
double calculateLatency(const vector<int>& chromosome, const Topology& network) {
    double total_latency = 0.0;
    int sink_packets = 0;
    int total_packets = network.total_packet_count;
    if(hasCycle(chromosome, network)){
        return DBL_MAX;
    }
    vector<int> held_packets(chromosome.size());
    for (size_t i = 0; i < chromosome.size(); i++) {
        held_packets[i]= network.node_list[i].packet_size;
    }
    while(sink_packets != total_packets){
        for (size_t i = 0; i < chromosome.size(); i++) {
            int src = i;
            int dst = chromosome[i];
            if (held_packets[src] == 0) continue;
            const Node& source = network.node_list[src];
            const Node& target = network.node_list[dst];
            double distance = source.distanceTo(target);
            double propagation_delay = distance / network.propagation_speed;
            double transmission_delay = (double)held_packets[src] / min(source.bandwidth, target.bandwidth);
            total_latency += propagation_delay + transmission_delay;
            
            if (target.isSink) {
                sink_packets += held_packets[src];
            } else {
                held_packets[dst] += held_packets[src];
            }
            held_packets[src] = 0;
        }
    }
    return total_latency;
}

double calculateEnergyUsage(const vector<int>& chromosome, Topology network) {
    double total_energy_used = 0.0;
    int sink_packets = 0;
    int total_packets = network.total_packet_count;

    if (hasCycle(chromosome, network)) {
        return DBL_MAX;
    }

    vector<int> held_packets(chromosome.size());
    for (size_t i = 0; i < chromosome.size(); i++) {
        held_packets[i] = network.node_list[i].packet_size;
    }

    while (sink_packets != total_packets) {
        for (size_t i = 0; i < chromosome.size(); i++) {
            int src = i;
            int dst = chromosome[i];

            if (held_packets[src] == 0) continue;

            Node& source = network.node_list[src];
            Node& target = network.node_list[dst];

            double distance = source.distanceTo(target);

            // Energy cost (can scale however you want, here proportional to distance and packets)
            double tx_energy = held_packets[src] * source.txCost * distance*distance/10;
            double rx_energy = 0;
            if(!target.isSink){
                rx_energy = held_packets[src] * target.rxCost; // e.g. receiver overhead
            }
            source.depleteEnergy(tx_energy);
            target.depleteEnergy(rx_energy);
            if(!source.isAlive || !target.isAlive){
                return DBL_MAX;
            }
            total_energy_used += tx_energy + rx_energy;

            if (target.isSink) {
                sink_packets += held_packets[src];
            } else {
                held_packets[dst] += held_packets[src];
            }
            held_packets[src] = 0;
        }
    }
    return total_energy_used;
}


double calculateThroughput(const vector<int>& chromosome, const Topology& network) {
    double throughput = 0.0;
    // Implement throughput calculation based on the chromosome and network topology
    return throughput;
}

bool validate_network_range(const vector<int>& chromosome, const Topology& network) {
    for(size_t i=0; i<chromosome.size();i++){
        int next_hop = chromosome[i];
        if(!network.node_list[i].isInRange(network.node_list[next_hop])){
            return false;
        }
    }
    return true;
}

double calculateFitness(const vector<int>& chromosome, Topology& network) {
    constexpr double c1 = 1.0;
    constexpr double c2 = 1.0;
    constexpr double c3 = 1.0;

    if(!validate_network_range(chromosome, network) || hasCycle(chromosome, network)){
        return DBL_MAX;
    }

    double energyUsage = calculateEnergyUsage(chromosome, network);
    double latency     = calculateLatency(chromosome, network);
    double throughput  = calculateThroughput(chromosome, network);

    return c1 * energyUsage + c2 * latency - c3 * throughput;
}

#endif