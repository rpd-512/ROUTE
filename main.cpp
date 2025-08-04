#include <vector>
#include "include/types.h"
#include "include/plotter_utils.h"

int main() {
    std::vector<Node> nodes;

    // Push a new Node
    nodes.push_back(Node(1, {0.0, 0.0}, 15.0, 100.0));
    nodes.push_back(Node(2, {5.0, 1.0}, 10.0, 55.0));
    nodes.push_back(Node(3, {4.0, -6.0}, 10.0, 47.0));
    nodes.push_back(Node(4, {-10.0, 10.0}, 5.0, 20)); // default energy = 100.0

    for (const auto& node : nodes) {
        node.printStatus();
    }
    plotNodes(nodes);
    return 0;
}
