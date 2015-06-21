#pragma once
#include <IEvolutionaryConfig.h>
#include <random>
#include "Random/Random.h"
#include <string>

namespace Darwin
{
	template<class GoalFunction, class Individual, class Population = std::vector<Individual>>
	class ProbabilisticEvolutionaryConfig: public Interfaces::IStandardEvolutionarConfig<GoalFunction, Individual, Population>
	{
		using base = Interfaces::IStandardEvolutionarConfig<Individual, Population>;
	public:
		// sampling: multinomial distribution
		using base::base;

		virtual typename base::individuals_references selectForCrossOver(typename base::population_type& population)
		{
			// multinomial by default
			// other methods: Tournament, SCX
		}

		virtual typename base::individuals_references selectForMutation(typename base::population_type& population)
		{
			// uniform by default
			// static output size
		}

		virtual typename base::individuals_references selectForRemoval(typename base::population_type& population)
		{
			// multinomial method by default
		}

		virtual void initializePopulation(typename base::population_type& population)
		{
			// distribution: uniform by default
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
