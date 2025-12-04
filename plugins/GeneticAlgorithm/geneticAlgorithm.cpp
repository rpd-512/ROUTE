#include "geneticAlgorithm.h"

PlotterData GeneticAlgorithm::algorithm_logic(indicators::BlockProgressBar& bar) {
    int eliteVal = max(1, this->population_size / 10);
    int crossValue = 3;
    float crossProb = 0.75;
    float mutateProb = 0.5;
    vector<ChromoInfo> popData;

    for(int u=0;u<population_size;u++){
        vector<int> valArr = population[u];
        ChromoInfo valChromo;
        valChromo.gene = valArr;
        valChromo.fitness = fitness(valArr, network, simulator);
        popData.push_back(valChromo);
    }
    ChromoInfo bestPop;
    vector<ChromoInfo> eliteData;
    for(int gen=0;gen<iteration_count+1;gen++){
        //-----------//
        bar.set_option(option::PostfixText{
        std::to_string(gen) + "/" + std::to_string(simulator.iteration_count)
        });
        bar.tick();
        //-----------//
        sort_population(&popData);
        popData = {popData.begin(), popData.begin()+population_size};
        eliteData.assign(popData.begin(), popData.begin() + eliteVal);

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
        }
        popData.insert(popData.end(),eliteData.begin(),eliteData.end());
    }
    return plotData;
}

// MUST EXPORT PLAIN C SYMBOL
extern "C" EvolutionEngine* create_algorithm() {
    cout << "Creating Genetic Algorithm Instance" << endl;
    return new GeneticAlgorithm();
}
