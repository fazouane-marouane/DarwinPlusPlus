#include <GeneticAlgorithms.h>
#include <GeneticAlgorithms/GeneticAlgorithmsImpl.h>

using namespace Darwin::Interfaces;

IEvolutionaryConfig& Darwin::GeneticAlgorithmLoop(IEvolutionaryConfig& config)
{
	config.init();
	while(! config.goalReached())
	{
		config.breed();
	}
	return config;
}
