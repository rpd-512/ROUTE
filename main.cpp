#include <iostream>
#include <vector>
#include "include/types.h"
#include "include/plotter_utils.h"
#include "include/io_utils.h"

#include "include/debug_utils.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <network.yaml>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    Topology myNetwork = loadNodesFromYAML(filename);

    //plotNodes(myNetwork);

    //Next hop encoding
    
    // Fitness Functions to be implemented:
    //      Minimizing Latency
    //      Minimizing Energy Usage
    //      Keeping Node Deaths to zero
    //      Pushing throughput to one

    // implement hashmap for node id to index mapping
    // chromosome size is not number of nodes, but number of nodes - sinks

    int populationSize = 10;
    //int iterations = 100;

    vector<vector<int>> population = initial_population(populationSize, myNetwork);
    cout << "Initial Population:" << endl;
    print_2D_vector(population);

    return 0;
}
