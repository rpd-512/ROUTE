#ifndef PLOTTER_UTILS_H
#define PLOTTER_UTILS_H

#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "matplotlibcpp.h"
#include "types.h"
#include "random_utils.h"

namespace plt = matplotlibcpp;

// Map energy to a color manually (very basic: high=green, low=red)
string energyToColor(double energy) {
    if (energy > 75) return "green";
    if (energy > 50) return "lightgreen";
    if (energy > 25) return "orange";
    if (energy > 0) return "red";
    return "black";
}

void plotCircle(double cx, double cy, double radius, bool line_flag = true) {
    const string& color = "black";
    vector<double> x, y;
    for (int i = 0; i <= 100; ++i) {
        double theta = 2 * M_PI * i / 100.0;
        x.push_back(cx + radius * cos(theta));
        y.push_back(cy + radius * sin(theta));
    }
    plt::plot(x, y, {{"color", color}});

    if(line_flag){
        double angle = randint(0,2) * 2*M_PI / 3; // Random angle for the radius line
        double edge_x = cx + radius * cos(angle);
        double edge_y = cy + radius * sin(angle);

        vector<double> line_x = {cx, edge_x};
        vector<double> line_y = {cy, edge_y};

        plt::plot(line_x, line_y, {
            {"color", color},
            {"linestyle", "--"}
        });
    }
}

void plotNodes(const Topology& network) {
    for (const auto& node : network.node_list) {

        vector<double> x{node.position.x};
        vector<double> y{node.position.y};
        int size = 50; // Size of the marker
        map<string, string> kwargs;
        if (node.isSink) {
            size = 150; // Sink nodes are larger circles
            kwargs["color"] = "blue";
            kwargs["marker"] = "s"; // Circle marker for sink nodes
        } else {
            kwargs["color"] = energyToColor(node.energy);
            plotCircle(node.position.x, node.position.y, node.radius);
        }
        plt::scatter(x, y, size, kwargs);
    }

    plt::title("Node Positions and Energy Levels");
    plt::xlabel("X Position");
    plt::ylabel("Y Position");
    plt::axis("equal");
    double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
    for (const auto& node : network.node_list) {
        minX = min(minX, node.position.x);
        maxX = max(maxX, node.position.x);
        minY = min(minY, node.position.y);
        maxY = max(maxY, node.position.y);
    }
    plt::xlim(minX - 10, maxX + 10);
    plt::ylim(minY - 10, maxY + 10);

    plt::show();
}

void plotConfig(const vector<int>& chromosome, const Topology& network){
    for (const auto& node : network.node_list) {

        vector<double> x{node.position.x};
        vector<double> y{node.position.y};
        int size = 50; // Size of the marker
        map<string, string> kwargs;
        if (node.isSink) {
            size = 150; // Sink nodes are larger circles
            kwargs["color"] = "blue";
            kwargs["marker"] = "s"; // Circle marker for sink nodes
        } else {
            kwargs["color"] = energyToColor(node.energy);
            plotCircle(node.position.x, node.position.y, node.radius, false);
        }
        plt::scatter(x, y, size, kwargs);
    }

    plt::title("Node Positions and Energy Levels");
    plt::xlabel("X Position");
    plt::ylabel("Y Position");
    plt::axis("equal");
    double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
    for (const auto& node : network.node_list) {
        minX = min(minX, node.position.x);
        maxX = max(maxX, node.position.x);
        minY = min(minY, node.position.y);
        maxY = max(maxY, node.position.y);
    }

    for(size_t i=0; i<chromosome.size();i++){
        Position cur_node = network.node_list[i].position;
        Position nex_node = network.node_list[chromosome[i]].position;
        vector<double> line_x = {cur_node.x, nex_node.x};
        vector<double> line_y = {cur_node.y, nex_node.y};

        plt::plot(line_x, line_y, {
            {"color", "black"},
            {"linestyle", "-"}
        });
    }

    plt::xlim(minX - 10, maxX + 10);
    plt::ylim(minY - 10, maxY + 10);

    plt::show();
}

#endif // PLOTTER_UTILS_H
