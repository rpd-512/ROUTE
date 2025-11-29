#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "../../include/algorithm_utils.h"

class GeneticAlgorithm : public EvolutionEngine {
public:
    int crossValue = 3;
    float crossProb = 0.75;
    float mutateProb = 0.5;
    int eliteVal = max(1, this->population_size / 10);

    //Existing variables in
    //  this->simulator
    //  this->network
    //  this->population_size
    //  this->iteration_count
    //  this->population

    GeneticAlgorithm(){
        this->setName("Genetic Algorithm");
        this->setColor("green");
    }
    
protected:
    PlotterData algorithm_logic(indicators::BlockProgressBar& bar) override;
};

extern "C" EvolutionEngine* create_algorithm();

#endif