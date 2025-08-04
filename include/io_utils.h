#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <yaml-cpp/yaml.h>
#include "types.h"

vector<Node> loadNodesFromYAML(const string& filename) {
    vector<Node> nodes;
    YAML::Node config = YAML::LoadFile(filename);

    for (const auto& nodeDef : config["nodes"]) {
        int id = nodeDef["id"].as<int>();
        auto posList = nodeDef["pos"];
        Position pos = { posList[0].as<double>(), posList[1].as<double>() };

        double energy = nodeDef["energy"].as<double>();
        string type = nodeDef["type"].as<string>();

        bool isSink = (type == "sink");
        double radius = isSink ? 9999.0 : nodeDef["range"] ? nodeDef["range"].as<double>() : 0.0;
        int packets = nodeDef["packets"] ? nodeDef["packets"].as<int>() : 0;
        double tx_depl = nodeDef["tx_depl"] ? nodeDef["tx_depl"].as<double>() : 0.0;
        double rx_depl = nodeDef["rx_depl"] ? nodeDef["rx_depl"].as<double>() : 0.0;

        Node node(id, pos, radius, energy, packets, tx_depl, rx_depl, isSink);
        nodes.push_back(node);
    }

    return nodes;
}

#endif