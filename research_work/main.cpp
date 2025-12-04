#include <iostream>
#include <vector>
#include <set>
#include "../include/types.h"
#include "../include/plotter_utils.h"
#include "../include/io_utils.h"
#include "../include/interface_utils.h"
#include "../include/algorithm_utils.h"

int main(){
    Topology network1,network2,network3;
    SimulationData simulator;
    network1 = loadNodesFromYAML("../../example_topologies/alpha_testing_topology.yaml");
    network2 = loadNodesFromYAML("../../example_topologies/random_20_4_20.yaml");
    network3 = loadNodesFromYAML("../../example_topologies/random_topo_100_5_20.yaml");

    EvolutionEngine* ga = loadAlgorithmFromFile("../../plugins/GeneticAlgorithm/genetic.so");
    EvolutionEngine* pso = loadAlgorithmFromFile("../../plugins/ParticleSwarmOptimization/particle.so");

    simulator.iteration_count = 500;
    simulator.population_size = 200;
    simulator.c_energy = 1.0;
    simulator.c_latency = 1.0;
    simulator.population = initial_population(simulator, network2);

    ga->loadData(simulator, network2);
    pso->loadData(simulator, network2);
    
    ga->run();
    pso->run();
    
    cout << "GA Results:" << endl;
    cout << "Best energy: " << ga->plotData.best_energy << endl;
    cout << "Best latency: " << ga->plotData.best_latency << endl;
    cout << "Best fitness: " << ga->plotData.best_fitness << endl;

    cout << "PSO Results:" << endl;
    cout << "Best energy: " << pso->plotData.best_energy << endl;
    cout << "Best latency: " << pso->plotData.best_latency << endl;
    cout << "Best fitness: " << pso->plotData.best_fitness << endl;

    plotConfig(ga->plotData.best_gene, network2);
    //plotConfig(pso->plotData.best_gene, network2);
    plt::figure();
    plt::plot(ga->plotData.fitness_history, {{"label", ga->plotData.name + " Fitness"}, {"color", ga->plotData.color}});
    plt::plot(pso->plotData.fitness_history, {{"label", pso->plotData.name + " Fitness"}, {"color", pso->plotData.color}});
    plt::xlabel("Iteration");
    plt::ylabel("Value");
    plt::title("GA vs PH-PSO Convergence");
    plt::legend();
    plt::show();    

    return 0;
}