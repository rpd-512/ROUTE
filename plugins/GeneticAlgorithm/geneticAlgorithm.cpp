#include "geneticAlgorithm.h"

//PlotterData GeneticAlgorithm::algorithm_logic(indicators::BlockProgressBar& bar) {
//    // --- your full GA logic here ---
//}

// MUST EXPORT PLAIN C SYMBOL
extern "C" EvolutionEngine* create_algorithm() {
    return new GeneticAlgorithm();
}
