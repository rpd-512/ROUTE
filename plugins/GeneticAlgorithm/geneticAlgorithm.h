#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "../../include/algorithm_utils.h"

class GeneticAlgorithm : public EvolutionEngine {
public:

    //Existing variables in
    //  this->simulator
    //  this->network
    //  this->population_size
    //  this->iteration_count
    //  this->population

    GeneticAlgorithm(){
        this->setName("Genetic Algorithm");
        this->setColor("red");
    }
    
protected:
    PlotterData algorithm_logic(indicators::BlockProgressBar& bar) override;
};

extern "C" EvolutionEngine* create_algorithm();

#endif