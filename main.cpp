#include <iostream>
#include <vector>
#include "include/types.h"
#include "include/plotter_utils.h"
#include "include/io_utils.h"
#include "include/interface_utils.h"

#include "include/debug_utils.h"

void clear_screen() {
    #ifdef _WIN32
        (void)system("cls");
    #else
        (void)system("clear");
    #endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <network.yaml>" << endl;
        return 1;
    }

    string filename = argv[1];
    Topology myNetwork = loadNodesFromYAML(filename);
    

    clear_screen();
    
    show_start();

    int populationSize = 10;
    int iterations = 10;

    //vector<vector<int>> population = initial_population(populationSize, myNetwork);
    //cout << "Initial Population:" << endl;
    //print_2D_vector(population);
    //cout << endl;
    //for(auto chromo: population){
    //    print_vector(chromo);
    //    cout << "Latency: " << calculateLatency(chromo,myNetwork) << endl;
    //    cout << "Energy : " << calculateEnergyUsage(chromo,myNetwork) << endl;
    //    cout << "Fitness: " << fitness(chromo,myNetwork) << endl << endl;
    //    plotConfig(chromo, myNetwork);
    //}

    rl_attempted_completion_function = completer;

    char* input;
    while ((input = readline("(ROUTE)~$ ")) != nullptr) {
        if (*input) add_history(input);

        string cmd = trim(input);
        free(input);
        if(cmd == "clear") clear_screen();
        if(cmd == "exit") break;
        if(cmd == "route") show_start();
    }
    return 0;
}