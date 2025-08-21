#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <yaml-cpp/yaml.h>
#include "types.h"

#include "debug_utils.h"

Topology loadNodesFromYAML(const string& filename) {
    Topology network;
    YAML::Node config = YAML::LoadFile(filename);
    vector<int> node_ids; // List of node IDs
    vector<int> sink_ids; // List of sink node IDs
    
    vector<Node> node_list;
    vector<Node> sink_list;
    
    auto env_data = config["environment"][0];
    network.propagation_speed = env_data["propagation_speed"].as<double>();

    for (const auto& nodeDef : config["nodes"]) {
        int id = nodeDef["id"].as<int>();
        auto posList = nodeDef["pos"];
        Position pos = { posList[0].as<double>(), posList[1].as<double>() };

        double energy = nodeDef["energy"].as<double>();
        string type = nodeDef["type"].as<string>();

        bool isSink = (type == "sink");
        if (isSink) {
            sink_ids.push_back(id);
        }
        else {
            node_ids.push_back(id);
        }
        double radius = isSink ? 9999.0 : nodeDef["range"] ? nodeDef["range"].as<double>() : 0.0;
        unsigned int packets = nodeDef["packets"] ? nodeDef["packets"].as<int>() : 0;
        network.total_packet_count += packets;
        double tx_depl = nodeDef["tx_depl"] ? nodeDef["tx_depl"].as<double>() : 0.0;
        double rx_depl = nodeDef["rx_depl"] ? nodeDef["rx_depl"].as<double>() : 0.0;

        Node node(id, pos, radius, energy, packets, tx_depl, rx_depl, isSink);
        if (isSink) {
            sink_list.push_back(node);
        } else {
            node_list.push_back(node);
        }
    }

    network.num_nodes = node_ids.size();
    network.num_sinks = sink_ids.size();
    network.indexing = vector<int>(network.num_nodes + network.num_sinks);
    for(size_t i = 0; i < network.num_sinks; i++) {
        network.indexing[i] = sink_ids[i];
        sink_list[i].index = i; // Set the index for the sink node
        network.node_list.push_back(sink_list[i]);
    }
    for(size_t i = 0; i < network.num_nodes; i++) {
        network.indexing[i + network.num_sinks] = node_ids[i];
        node_list[i].index = i + network.num_sinks; // Set the index for the regular node
        network.node_list.push_back(node_list[i]);
    }

    return network;
}

#endif