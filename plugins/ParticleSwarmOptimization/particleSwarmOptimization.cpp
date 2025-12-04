#include "particleSwarmOptimization.h"

PlotterData ParticleSwarmOptimization::algorithm_logic(indicators::BlockProgressBar& bar) {
    float inertia = 0.7;
    float cognitive = 1.4;
    float social = 1.4;

    vector<Particle> popData;


    for (auto &chromo : population) {
        Particle p;
        double f = fitness(chromo, network, simulator);
        // initialize using our header helper or manually:
        p.chromo.gene = chromo;
        p.chromo.fitness = f;
        p.pbest = chromo;
        p.velocity.resize(network.num_nodes);
        // initialize each dim independently
        for (size_t d = 0; d < network.num_nodes; ++d)
            p.velocity[d] = uniform(-1.0f, 1.0f);
        popData.push_back(std::move(p));
    }
    
    Particle gbest = popData[0];

    for(int gen=0;gen<iteration_count+1;gen++){
        //-----------//
        bar.set_option(option::PostfixText{
        std::to_string(gen) + "/" + std::to_string(simulator.iteration_count)
        });
        bar.tick();
        //-----------//
        sort_particles(&popData);
    
        gbest = popData[0];
        gbest.chromo.fitness = fitness(gbest.chromo.gene, network, simulator);
    
        if(gbest.chromo.fitness < plotData.best_fitness){
            plotData.best_fitness = gbest.chromo.fitness;
            plotData.best_gene = gbest.chromo.gene;
            plotData.best_latency = calculateLatency(gbest.chromo.gene, network);
            plotData.best_energy = calculateEnergyUsage(gbest.chromo.gene, network);
        }
    
        plotData.fitness_history.push_back(plotData.best_fitness);
        plotData.latency_history.push_back(plotData.best_latency);
        plotData.energy_history.push_back(plotData.best_energy);
    
        //Implement PSO Logic Ahead
        for(int p = 0; p < population_size; p++){
            Particle &par = popData[p];
            
            for(size_t d = 0; d < network.num_nodes; d++){
                float r1 = uniform(0.0, 1.0);
                float r2 = uniform(0.0, 1.0);

                par.velocity[d] =
                    inertia * par.velocity[d] +
                    cognitive * r1 * (par.pbest[d] - par.chromo.gene[d]) +
                    social    * r2 * (gbest.chromo.gene[d] - par.chromo.gene[d]);

                // Clamp velocity
                if(par.velocity[d] > 2) par.velocity[d] = 2;
                if(par.velocity[d] < -2) par.velocity[d] = -2;

                float sig = 1.f / (1.f + exp(-par.velocity[d]));
                if(uniform(0.f,1.f) < sig)
                    par.chromo.gene[d] = gbest.chromo.gene[d];
                else
                    par.chromo.gene[d] = par.pbest[d];
            }

            // Repair routing chromosome if needed
            //repair_chromosome(par.chromo.gene, network);

            // Fitness evaluation
            par.chromo.fitness = fitness(par.chromo.gene, network, simulator);

            // pbest update
            if(par.chromo.fitness < fitness(par.pbest, network, simulator)){
                par.pbest = vector<int>(par.chromo.gene.begin(), par.chromo.gene.end());
            }
        }
    }
    return plotData;
}

// MUST EXPORT PLAIN C SYMBOL
extern "C" EvolutionEngine* create_algorithm() {
    cout << "Creating Particle Swarm Optimization Instance" << endl;
    return new ParticleSwarmOptimization();
}