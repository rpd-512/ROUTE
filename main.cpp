#include <iostream>
#include <vector>
#include "include/types.h"
#include "include/plotter_utils.h"
#include "include/io_utils.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <network.yaml>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<Node> myNetwork = loadNodesFromYAML(filename);

    for (const auto& node : myNetwork) {
        node.printStatus();
    }

    plotNodes(myNetwork);
    return 0;
}
