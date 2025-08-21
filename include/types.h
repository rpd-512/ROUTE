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
    unsigned int packetCount; // Number of packets the node has to send
    double txCost; // Transmission cost per packet
    double rxCost; // Reception cost per packet
    bool isSink = false; // Default to false, can be set later

    Node(int id,
         Position position,
         double radius,
         double energy = 100.0,
         int packetCount = 512,
         double txCost = 1.0,
         double rxCost = 1.0,
         bool isSink = false)
        : id(id),
          position(position),
          radius(radius),
          energy(energy < 0 ? 0 : energy),
          isAlive(true),
          packetCount(packetCount),
          txCost(txCost),
          rxCost(rxCost),
          isSink(isSink) {}
    
    double distanceTo(const Node& other) const {
        double dx = other.position.x - position.x;
        double dy = other.position.y - position.y;
        return std::sqrt(dx * dx + dy * dy);
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
        energy = 100.0; // Reset energy to default value
        isAlive = true; // Reset alive status
    }
    void printStatus() const {
        std::cout << "Node ID: " << id
                  << "\n\t- Index   : " << index
                  << "\n\t- Position: (" << position.x << ", " << position.y << ")"
                  << "\n\t- Energy  : " << energy
                  << "\n\t- Is Alive: " << (isAlive ? "Yes" : "No")
                  << "\n\t- Packets : " << packetCount
                  << "\n\t- TX Cost : " << txCost
                  << "\n\t- RX Cost : " << rxCost
                  << "\n\t- Is Sink : " << (isSink ? "Yes" : "No")
                  << "\n\t- Radius  : " << radius
                  << std::endl;
    }
};

class Topology {
public:
    unsigned int total_packet_count;
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
};


#endif