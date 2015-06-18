#pragma once
#include <IEvolutionaryConfig.h>
#include <random>
#include "Random/Random.h"
#include <string>

namespace Darwin
{
	template<class Individual, class Population>
	class ProbabilisticEvolutionaryConfig: public Interfaces::IStandardEvolutionarConfig<Individual, Population>
	{
		using base = Interfaces::IStandardEvolutionarConfig<Individual, Population>;
	public:
		virtual typename base::individuals_references selectForCrossOver(typename base::population_type& population)
		{

		}

		virtual typename base::individuals_references selectForMutation(typename base::population_type& population)
		{

		}

		virtual typename base::individuals_references selectForRemoval(typename base::population_type& population)
		{

		}

		virtual void initializePopulation(typename base::population_type& population)
		{

		}
	private:
		Rand::uniform_distribution<Individual> dist_crossOver;
		Rand::uniform_distribution<Individual> dist_mutation;
		Rand::uniform_distribution<Individual> dist_removal;
		Rand::uniform_distribution<Individual> dist_initialization;
		std::random_device rd;
		std::mt19937 gen;
	};
}
