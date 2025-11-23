#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "../include/algorithm_utils.h"

class GeneticAlgorithm : public EvolutionEngine {
public:
    int crossValue = 3;
    float crossProb = 0.75;
    float mutateProb = 0.5;
    int eliteVal = max(1, this->population_size / 10);

    GeneticAlgorithm(){
        this->setName("Genetic Algorithm");
        this->setColor("green");
    }
    
protected:
    PlotterData algorithm_logic(indicators::BlockProgressBar& bar) override {
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
        return plotData;
    }
};

#endif