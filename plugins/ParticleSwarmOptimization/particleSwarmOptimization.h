#ifndef PARTICLE_SWARM_OPTIMIZATION_H
#define PARTICLE_SWARM_OPTIMIZATION_H

#include "../../include/algorithm_utils.h"

class ParticleSwarmOptimization : public EvolutionEngine {
public:

    //Existing variables in
    //  this->simulator
    //  this->network
    //  this->population_size
    //  this->iteration_count
    //  this->population

    struct Particle{
        ChromoInfo chromo;
        vector<int> pbest;
        vector<float> velocity;
    };

    ParticleSwarmOptimization(){
        this->setName("Particle Swarm Optimization");
        this->setColor("blue");
    }
    
protected:
    PlotterData algorithm_logic(indicators::BlockProgressBar& bar) override;

    void sort_particles(vector<Particle>* popData){
        sort(popData->begin(), popData->end(), [](const Particle& a, const Particle& b) {
            return a.chromo.fitness < b.chromo.fitness;
        });
    }

};

extern "C" EvolutionEngine* create_algorithm();

#endif