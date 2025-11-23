#ifndef ALGORITHM_UTILS_H
#define ALGORITHM_UTILS_H

#include "types.h"
#include "random_utils.h"
#include "network_utils.h"
#include "plotter_utils.h"
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <thread>
#include <csignal>
#include <cstdlib>

using namespace indicators;

class EvolutionEngine {
public:
    EvolutionEngine(){}
    
    virtual void setName(const string& algo_name){
        this->plotData.name = algo_name;
    }

    virtual void setColor(const string& line_color){
        this->plotData.color = line_color;
    }

    void loadData(SimulationData simulator, Topology network, bool return_history = true) {
        this->simulator = simulator;
        this->network = network;
        this->return_history = return_history;
        this->population_size = simulator.population_size;
        this->iteration_count = simulator.iteration_count;
        this->population = simulator.population;
        this->plotData.hash_id = random_string(7);
        this->plotData.best_fitness = FLT_MAX;
        this->plotData.best_gene = vector<int>(network.num_nodes, -1);
        this->plotData.fitness_history.reserve(iteration_count);
        this->plotData.latency_history.reserve(iteration_count);
        this->plotData.energy_history.reserve(iteration_count);
    }

    void run() {
        indicators::show_console_cursor(false);
        BlockProgressBar bar{
            option::BarWidth{40},
            option::ForegroundColor{Color::white},
            option::PrefixText{this->plotData.name + " "},
            option::ShowElapsedTime{true},
            option::Start{"|"},
            option::End{"|"},
            option::ShowRemainingTime{true},
            option::FontStyles{
                std::vector<FontStyle>{FontStyle::bold}},
            option::MaxProgress{simulator.iteration_count}
        };

        plotData = algorithm_logic(bar);

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

protected:
    int population_size;
    int iteration_count;
    vector<vector<int>> population;
    PlotterData plotData;
    Topology network;
    SimulationData simulator;
    vector<ChromoInfo> popData;
    bool return_history;

    void sort_population() {
        sort(popData.begin(), popData.end(), [](const ChromoInfo& a, const ChromoInfo& b) {
            return a.fitness < b.fitness;
        });
    }


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

    virtual PlotterData algorithm_logic(indicators::BlockProgressBar& bar){
        PlotterData default_return;
        cout << "Please create the an overriding logic before executing the algorithm\n";
        return default_return;
    }
};

#endif