#pragma once
#include <list> // std::list
#include <vector>
#include <functional> // std::reference_wrapper

namespace Darwin
{
	namespace Interfaces
	{
		class IEvolutionaryConfig
		{
		public:
			virtual IEvolutionaryConfig& init() = 0;
			virtual IEvolutionaryConfig& breed() = 0;
			virtual bool goalReached() = 0;
		};

		template<class GoalFunction, class Individual, class Population= std::vector<Individual>>
		class IStandardEvolutionaryConfig: public IEvolutionaryConfig
		{
		public:
			using individual_type = Individual;
			using population_type = Population;
			using individuals_references = std::list<std::reference_wrapper<Individual>>; // TODO: lame naming

			IStandardEvolutionaryConfig(GoalFunction _goalFunction) : goalFunction(_goalFunction) {}

			virtual IEvolutionaryConfig& init()
			{
				initializePopulation(Population);
				return *this;
			}

			virtual IEvolutionaryConfig& breed()
			{
				// Cross Over
				auto newIndividuals = crossOver(selectForCrossOver(population));
				// Mutation
				auto mutants = mutate(selectForMutation(population));
				// Merge these new individuals into the original population
				population.add(newIndividuals, mutants); // TODO: sort this, somehow
				// Natural selection
				population.remove(selectForRemoval(population));
				return *this;
			}

			virtual individuals_references selectForCrossOver(population_type&, method = 'multinomial') = 0;

			virtual individuals_references selectForMutation(population_type&, method = 'multinomial') = 0;

			virtual individuals_references selectForRemoval(population_type&, method = 'multinomial') =0;

			virtual void initializePopulation(population_type&) =0;

			virtual population_type crossOver(individuals_references&) =0;

			virtual population_type mutate(individuals_references&) =0;

		protected:
			population_type population;
			GoalFunction goalFunction;
		};
        

	}
}
