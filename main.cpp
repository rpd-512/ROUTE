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

    cout << "\nINDEX\tID\tType" << endl;
    for(size_t i=0;i<myNetwork.indexing.size();i++) {
        cout << i << "\t" << myNetwork.indexing[i] << "\t";
        if(i < myNetwork.num_sinks) {
            cout << "Sink";
        } else {
            cout << "Node";
        }
        cout << endl;
    }
    cout << endl;

    for(size_t i=0;i<myNetwork.node_list.size();i++) {
        myNetwork.node_list[i].printStatus();
    }
    cout << endl;
    myNetwork.generate_adjacency_matrix();
    cout << "Adjacency Matrix:" << endl;
    print_2D_vector(myNetwork.adjacency_matrix);

    //Next hop encoding
    
    // Fitness Functions to be implemented:
    //      Minimizing Latency
    //      Minimizing Energy Usage
    //      Keeping Node Deaths to zero
    //      Pushing throughput to one

    // chromosome size is not number of nodes, but number of nodes - sinks

    int populationSize = 10;
    int iterations = 100;

    vector<vector<int>> population = initial_population(populationSize, myNetwork);
    cout << "Initial Population:" << endl;
    print_2D_vector(population);

    plotNodes(myNetwork);

    return 0;
}
