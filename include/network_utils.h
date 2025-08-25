#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "types.h"
#include <cfloat>
#include <unordered_set>
#include "debug_utils.h"
#include "random_utils.h"


int randint(int low, int high);
double uniform(double low, double high);

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
            double tx_energy = held_packets[src] * source.txCost;
            double rx_energy = 0;
            if(!target.isSink){
                rx_energy = held_packets[src] * target.rxCost * distance*distance/100; // e.g. receiver overhead
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

bool validate_network_range(const vector<int>& chromosome, const Topology& network) {
    for(size_t i=0; i<chromosome.size();i++){
        int next_hop = chromosome[i];
        if(!network.node_list[i].isInRange(network.node_list[next_hop])){
            return false;
        }
    }
    return true;
}

double fitness(const vector<int>& chromosome, Topology& network, SimulationData simulator) {
    double c1 = simulator.c_energy;
    double c2 = simulator.c_latency;

    if(!validate_network_range(chromosome, network) || hasCycle(chromosome, network)){
        return DBL_MAX;
    }

    double energyUsage = calculateEnergyUsage(chromosome, network);
    double latency     = calculateLatency(chromosome, network);

    //minimise
    return c1 * energyUsage + c2 * latency;
}

vector<int> find_cycle_nodes(const vector<int>& chromosome, int start) {
    unordered_set<int> visited;
    vector<int> path;

    int node = start;
    while (node >= 0 && node < (int)chromosome.size()) {
        if (visited.count(node)) {
            // Cycle detected: collect cycle nodes
            auto it = find(path.begin(), path.end(), node);
            return vector<int>(it, path.end());
        }
        visited.insert(node);
        path.push_back(node);
        node = chromosome[node];
    }
    return {}; // no cycle
}

void break_cycle(vector<int>& chromosome, const Topology& network) {
    for (int i = 0; i < (int)chromosome.size(); i++) {
        auto cycle_nodes = find_cycle_nodes(chromosome, i);
        if (!cycle_nodes.empty()) {
            // Pick one node in the cycle to redirect
            int node_to_fix = cycle_nodes[randint(0, cycle_nodes.size()-1)];

            // Redirect to a sink if possible
            if (network.num_sinks > 0) {
                int sink = network.num_nodes + randint(0, network.num_sinks-1);
                chromosome[node_to_fix] = sink;
            } else {
                // fallback: random node
                chromosome[node_to_fix] = randint(0, network.num_nodes-1);
            }
        }
    }
}

Topology generate_random_topology(int num_nodes, int num_sinks, double area_size) {
    Topology network;
    vector<int> node_ids; // List of node IDs
    vector<int> sink_ids; // List of sink node IDs
    network.num_nodes = num_nodes;
    network.num_sinks = num_sinks;
    network.indexing = vector<int>(network.num_nodes + network.num_sinks);
    network.propagation_speed = 3e8; // Speed of light in m/s
    cout << "Generating random topology with " << num_nodes << " nodes and " << num_sinks << " sinks." << endl;
    int placed_sinks = 0;
    int placed_nodes = 0;

    // Place the first regular node anywhere in the area
    Position pos = {uniform(-area_size/2, area_size/2),
                    uniform(-area_size/2, area_size/2)};
    Node first_node(
        randint(0, num_nodes*10), // next index
        pos,
        uniform(0, area_size/4),   // transmission range
        sqrt(uniform(0, 10000)),           // initial energy
        randint(0, 32),            // channel
        uniform(0.01, 0.2),
        uniform(0.01, 0.2),
        false,                     // is_sink
        uniform(1, 100)          // processing capacity
    );
    node_ids.push_back(first_node.id);
    network.node_list.push_back(first_node);
    placed_nodes++;
    network.total_packet_count = first_node.packet_size;

    // Place rest of the nodes
    for (int i = 1; i < num_nodes; i++) {
        // Pick a random "parent" node to place near
        Node parent = network.node_list[randint(0, network.node_list.size() - 1)];
        
        // Generate position nearby parent (small random offset)
        Position pos = {
            parent.position.x + uniform(-area_size/10, area_size/10),
            parent.position.y + uniform(-area_size/10, area_size/10)
        };

        // Clamp to area bounds
        pos.x = max(-area_size/2, min(area_size/2, pos.x));
        pos.y = max(-area_size/2, min(area_size/2, pos.y));


        Node node(
            randint(0, num_nodes*10), // next index
            pos,
            uniform(0, area_size/10),
            sqrt(uniform(0, 10000)),           // initial energy
            randint(0, 32),
            uniform(0.01, 0.2),
            uniform(0.01, 0.2),
            false,
            uniform(1, 100)
        );
        node_ids.push_back(node.id);
        node.index = network.node_list.size();
        network.total_packet_count += node.packet_size;

        network.node_list.push_back(node);

        placed_nodes++;
    }

    // If we’re missing sinks, add them now
    while (placed_sinks < num_sinks) {
        Node parent = network.node_list[randint(0, network.node_list.size() - 1)];
        Position pos = {
            parent.position.x + uniform(-area_size/10, area_size/10),
            parent.position.y + uniform(-area_size/10, area_size/10)
        };

        pos.x = max(-area_size/2, min(area_size/2, pos.x));
        pos.y = max(-area_size/2, min(area_size/2, pos.y));

        Node sink(
            randint(0, num_nodes*10),
            pos,
            uniform(0, area_size/4),
            9999,
            randint(0, 32),
            0,
            0,
            true, // sink
            uniform(10, 1000)
        );
        sink_ids.push_back(sink.id);
        sink.index = network.node_list.size();

        network.node_list.push_back(sink);
        placed_sinks++;
    }

    for(size_t i = 0; i < network.num_nodes; i++) {
        network.indexing[i] = node_ids[i];
    }
    for(size_t i = 0; i < network.num_sinks; i++) {
        network.indexing[i + network.num_nodes] = sink_ids[i];
    }
    network.generate_adjacency_matrix();

    while(network.isolated_node_cycles()) {
        Node& node = network.node_list[randint(0, network.num_nodes - 1)];
        node.radius+=uniform(0, area_size/100);
        network.generate_adjacency_matrix();
    }
    network.generate_adjacency_matrix();
    cout << "Successfully generated valid topology." << endl;
    return network;
}

#endif