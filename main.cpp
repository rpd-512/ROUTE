#include <iostream>
#include <vector>
#include <set>
#include "include/types.h"
#include "include/plotter_utils.h"
#include "include/io_utils.h"
#include "include/interface_utils.h"

#include "include/debug_utils.h"

#include "metaheuristics/geneticAlgorithm.h"

int main(int argc, char* argv[]) {
    Topology myNetwork;
    SimulationData simulator;

    simulator.iteration_count = 1000;
    simulator.population_size = 100;
    simulator.c_energy = 1.0;
    simulator.c_latency = 1.0;


    clear_screen();
    show_start();

    rl_attempted_completion_function = completer;

    char* input;
    while ((input = readline("(ROUTE)~$ ")) != nullptr) {
        if (*input) add_history(input);

        execute_command(input, myNetwork, simulator);
    }
    return 0;
}