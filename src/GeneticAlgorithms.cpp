#pragma once
#include <DarwinConfig.h>
#ifndef DARWIN_INLINE
#include <GeneticAlgorithms.h>
#endif
#include <GeneticAlgorithms/GeneticAlgorithmsImpl.h>
#include <IEvolutionaryConfig.h>


DARWIN_FUNCTION Darwin::Interfaces::IEvolutionaryConfig& Darwin::GeneticAlgorithmLoop(Darwin::Interfaces::IEvolutionaryConfig& config)
{
	config.init();
	while(! config.goalReached())
	{
		config.breed();
	}
	return config;
}
