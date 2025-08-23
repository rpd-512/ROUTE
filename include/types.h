#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <iostream>

using namespace std;

struct Position {
    double x, y;
};

class Node {
public:
    unsigned int id; // Unique identifier for the node read from YAML file
    unsigned int index; // Index in the network's node_list
    Position position; // Position of the node in 2D space
    double radius; // Communication range of the node
    double energy; // Current energy level of the node
    bool isAlive; // Status of the node (alive or dead)
    unsigned int packet_size; // Number of packets the node has to send
    double txCost; // Transmission cost per packet
    double rxCost; // Reception cost per packet
    bool isSink = false; // Default to false, can be set later
    double bandwidth;

    Node(int id,
         Position position,
         double radius,
         double energy = 100.0,
         int packet_size = 512,
         double txCost = 1.0,
         double rxCost = 1.0,
         bool isSink = false,
         double bandwidth = 2e6
        )
        : id(id),
          position(position),
          radius(radius),
          energy(energy < 0 ? 0 : energy),
          isAlive(true),
          packet_size(packet_size),
          txCost(txCost),
          rxCost(rxCost),
          isSink(isSink),
          bandwidth(bandwidth) {
            initial_energy = energy;
          }
    
    double distanceTo(const Node& other) const {
        double dx = other.position.x - position.x;
        double dy = other.position.y - position.y;
        return sqrt(dx * dx + dy * dy);
    }

    bool isInRange(const Node& other) const {
        return distanceTo(other) <= radius && other.isAlive && isAlive;
    }

    void depleteEnergy(double amount) {
        energy -= amount;
        if (energy <= 0) {
            energy = 0;
            isAlive = false;
        }
    }
    void reset() {
        energy = initial_energy; // Reset energy to default value
        isAlive = (energy>0); // Reset alive status
    }
    void printStatus() const {
        cout << "Node ID: " << id
                  << "\n\t- Index     : " << index
                  << "\n\t- Position  : (" << position.x << ", " << position.y << ")"
                  << "\n\t- Energy    : " << energy
                  << "\n\t- Is Alive  : " << (isAlive ? "Yes" : "No")
                  << "\n\t- Bandwidth : " << bandwidth
                  << "\n\t- Packets   : " << packet_size
                  << "\n\t- TX Cost   : " << txCost
                  << "\n\t- RX Cost   : " << rxCost
                  << "\n\t- Is Sink   : " << (isSink ? "Yes" : "No")
                  << "\n\t- Radius    : " << radius
                  << endl;
    }
private:
    double initial_energy;
    
};

class Topology {
public:
    unsigned int total_packet_count = 0;
    double propagation_speed;
    vector<Node> node_list; // List of nodes in the network in order of index
    vector<vector<int>> adjacency_matrix; // Adjacency matrix for the network
    vector<int> indexing; // Maps node ID to its index in the node_list
    unsigned int num_nodes; // Total number of non-sink nodes in the network
    unsigned int num_sinks; // Total number of sink nodes in the network
    Topology() : num_nodes(0), num_sinks(0) {}
    
    void generate_adjacency_matrix() {
        adjacency_matrix.resize(node_list.size(), vector<int>(node_list.size(), 0));
        for (size_t i = 0; i < node_list.size(); ++i) {
            for (size_t j = 0; j < node_list.size(); ++j) {
                if (i != j && node_list[i].isInRange(node_list[j])) {
                    adjacency_matrix[i][j] = 1; // Mark as connected
                }
            }
        }
    }

    void print_network() const {
        for(const auto& node : node_list) {
            node.printStatus();
        }
    }

    void print_environment_data() const{
        cout << "Propogation Speed : " << propagation_speed << endl;
        cout << "Total Packet Count: " << total_packet_count << endl;
    }

    vector<int> get_reachable_nodes(int source_index) {
        vector<int> reachable_nodes;

        if (source_index >= adjacency_matrix.size()) {
            return reachable_nodes; // out of bounds
        }

        for (size_t target_index = 0; target_index < adjacency_matrix[source_index].size(); ++target_index) {
            if (adjacency_matrix[source_index][target_index] == 1) {
                reachable_nodes.push_back(static_cast<int>(target_index));
            }
        }

        return reachable_nodes;
    }

    vector<int> get_incoming_nodes(int target_index) {
        vector<int> incoming_nodes;

        if (target_index >= adjacency_matrix.size()) {
            return incoming_nodes; // out of bounds
        }
        for (size_t source_index = 0; source_index < adjacency_matrix.size(); ++source_index) {
            if (target_index < adjacency_matrix[source_index].size() &&
                adjacency_matrix[source_index][target_index] == 1) {
                if(!node_list[source_index].isSink){
                    incoming_nodes.push_back(source_index);
                }
            }
        }

        return incoming_nodes;
    }

    bool isolated_node_cycles() {
        // For each non-sink node, check if it eventually reaches a sink
        for (size_t i = 0; i < node_list.size(); ++i) {
            if (node_list[i].isSink) continue; // skip sinks

            vector<bool> visited(node_list.size(), false);
            vector<int> stack;
            stack.push_back(i);

            bool reaches_sink = false;

            while (!stack.empty()) {
                int current = stack.back();
                stack.pop_back();

                if (visited[current]) continue;
                visited[current] = true;

                if (node_list[current].isSink) {
                    reaches_sink = true;
                    break;
                }

                for (int neighbor : get_reachable_nodes(current)) {
                    if (!visited[neighbor]) {
                        stack.push_back(neighbor);
                    }
                }
            }
            if (!reaches_sink) {
                return true;
            }
        }

        return false;
    }
    
    void reset_network(){
        for(Node &n: node_list){
            n.reset();
        }
    }
};


#endif