#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "../include/types.h"
#include "../include/random_utils.h"
#include "../include/network_utils.h"
#include "../include/plotter_utils.h"
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <thread>
#include <csignal>
#include <cstdlib>

class GeneticAlgorithm {
public:
    int crossValue;
    float crossProb;
    float mutateProb;
    int eliteVal;

    GeneticAlgorithm(SimulationData simulator, Topology network, bool return_history = true) {
        this->simulator = simulator;
        this->network = network;
        this->return_history = return_history;
        this->population_size = simulator.population_size;
        this->iteration_count = simulator.iteration_count;
        this->population = simulator.population;
        this->plotData.hash_id = random_string(5);
        this->plotData.name = "Genetic Algorithm";
        this->plotData.best_fitness = FLT_MAX;
        this->plotData.best_gene = vector<int>(network.num_nodes, -1);
        this->plotData.fitness_history.reserve(iteration_count);
        this->plotData.latency_history.reserve(iteration_count);
        this->plotData.energy_history.reserve(iteration_count);
        this->plotData.color = "red";
        crossValue = 3;
        crossProb = 0.75;
        mutateProb = 0.5;
        eliteVal = max(1, this->population_size / 10);
        run();
    }

    void sort_population() {
        sort(popData.begin(), popData.end(), [](const ChromoInfo& a, const ChromoInfo& b) {
            return a.fitness < b.fitness;
        });
    }

    void run() {
        indicators::show_console_cursor(false);
        using namespace indicators;
        BlockProgressBar bar{
            option::BarWidth{40},
            option::ForegroundColor{Color::white},
            option::PrefixText{"Genetic Algorithm "},
            option::ShowElapsedTime{true},
            option::Start{"|"},
            option::End{"|"},
            option::ShowRemainingTime{true},
            option::FontStyles{
                std::vector<FontStyle>{FontStyle::bold}},
            option::MaxProgress{simulator.iteration_count}
        };



        for(int u=0;u<population_size;u++){
            vector<int> valArr = population[u];
            ChromoInfo valChromo;
            valChromo.gene = valArr;
            valChromo.fitness = fitness(valArr, network, simulator);
            popData.push_back(valChromo);
        }
        ChromoInfo bestPop;
        for(int gen=0;gen<iteration_count+1;gen++){
            //-----------//
            bar.set_option(option::PostfixText{
            std::to_string(gen) + "/" + std::to_string(simulator.iteration_count)
            });
            bar.tick();
            //-----------//

            sort_population();
            popData = {popData.begin(), popData.begin()+population_size};
            vector<ChromoInfo> eliteData = {popData.begin(), popData.begin()+eliteVal};
            bestPop = popData[0];
            if(bestPop.fitness < plotData.best_fitness){
                plotData.best_fitness = bestPop.fitness;
                plotData.best_gene = bestPop.gene;
                plotData.best_latency = calculateLatency(bestPop.gene, network);
                plotData.best_energy = calculateEnergyUsage(bestPop.gene, network);
            }
            plotData.fitness_history.push_back(plotData.best_fitness);
            plotData.latency_history.push_back(plotData.best_latency);
            plotData.energy_history.push_back(plotData.best_energy);
            //Implement GA Logic Ahead
            for(int p=0; p<population_size; p++){
                vector<int>& chromoMain = popData[p].gene;
                int rand_p =randint(0,population_size-1);
                vector<int>& chromoRand = popData[rand_p].gene;
                int crossOverNum = randint(0,population[p].size() - crossValue);
                //crossover
                if(uniform(0,1) < crossProb){
                    swap_ranges(chromoMain.begin() + crossOverNum,
                            chromoMain.begin() + crossOverNum + crossValue,
                            chromoRand.begin() + crossOverNum);
                }
                for(int g=crossOverNum;g<crossOverNum+crossValue;g++){
                    if(uniform(0,1)<mutateProb){
                        int reachable_index = randint(0, network.get_reachable_nodes(g).size()-1);
                        chromoMain[g] = network.get_reachable_nodes(g)[reachable_index];
                    }
                }
                popData[p].fitness = fitness(chromoMain, network, simulator);
                //print_vector(chromoMain);
                //plotConfig(chromoMain, network);
            }
            popData.insert(popData.end(),eliteData.begin(),eliteData.end());
        }
        indicators::show_console_cursor(true);
        plot_container.push_back(plotData);
        cout << "\nBest Fitness: " << plotData.best_fitness << endl;
        cout << "Best Gene: ";
        print_vector(plotData.best_gene);
        cout << "Best Latency: " << plotData.best_latency << endl;
        cout << "Best Energy: " << plotData.best_energy << endl;
        if(return_history){
            cout << "Fitness History and other data stored. Use 'plot convergence' commands to visualize." << endl;
        }
        else{
            cout << "Fitness History and other data not stored. To store, set return_history to true." << endl;
        }
    }
    void plot_convergence() {
        cout << "Best Fitness: " << plotData.best_fitness << endl;
        cout << "Best Gene: ";
        print_vector(plotData.best_gene);
        plotConfig(plotData.best_gene, network);
        plotFitness(plotData);
        plotLatency(plotData);
        plotEnergy(plotData);
    }

private:
    int population_size;
    int iteration_count;
    vector<vector<int>> population;
    PlotterData plotData;
    Topology network;
    SimulationData simulator;
    vector<ChromoInfo> popData;
    bool return_history;

    void repair_chromosome(vector<int>& chromosome, const Topology& network) {
        // Fix invalid nodes first
        for (int i = 0; i < (int)chromosome.size(); i++) {
            if (chromosome[i] < 0 || chromosome[i] >= (int)network.num_nodes) {
                chromosome[i] = randint(0, network.num_nodes-1);
            }
        }
        // Break cycles until none remain
        while (hasCycle(chromosome, network)) {
            break_cycle(chromosome, network);
        }
    }
};

#endif