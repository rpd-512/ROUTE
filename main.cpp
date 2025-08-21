#include <iostream>
#include <vector>
#include "include/types.h"
#include "include/plotter_utils.h"
#include "include/io_utils.h"

#include "include/debug_utils.h"

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <network.yaml>" << std::endl;
        return 1;
    }
    clear_screen();
    cout << " ____   ___  _   _ _____ _____ " << endl;
    cout << "|  _ \\ / _ \\| | | |_   _| ____|" << endl;
    cout << "| |_) | | | | | | | | | |  _|  " << endl;
    cout << "|  _ <| |_| | |_| | | | | |___ " << endl;
    cout << "|_| \\_\\\\___/ \\___/  |_| |_____|" << endl;

    cout << "Route Optimization Using Tunable Evolution" << endl;

    string filename = argv[1];
    Topology myNetwork = loadNodesFromYAML(filename);

    //myNetwork.print_network();

    //myNetwork.generate_adjacency_matrix();
    
    //myNetwork.print_environment_data();

    int populationSize = 10;
    int iterations = 100;

    vector<vector<int>> population = initial_population(populationSize, myNetwork);
    cout << "Initial Population:" << endl;
    print_2D_vector(population);

    //plotNodes(myNetwork);

    return 0;
}
