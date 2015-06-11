#pragma once
#include <list> // std::list
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

		template<class Individual, class Population>
		class IStandardEvolutionarConfig: IEvolutionaryConfig
		{
		public:
			typedef Individual individual_type; // turn this into a c++11 Type aliase
			typedef Population population_type;
			using container_of_reference = std::list<std::reference_wrapper<Individual>>; // TODO: lame naming

			virtual IEvolutionaryConfig& init()
			{
				initializePopulation(population);
				return *this;
			}

			virtual IEvolutionaryConfig& breed()
			{
				// Cross Over
				auto newIndividuals = crossOver(selectForCrossOver(population));
				// Mutation
				auto mutants = mutate(selectForMutation(population));
				// NaturalSelection
				population.add(newIndividuals, mutants);
				population.remove(selectForRemoval(population));
				return *this;
			}

			virtual container_of_reference selectForCrossOver(population_type&) = 0;

			virtual container_of_reference selectForMutation(population_type&) = 0;

			virtual container_of_reference selectForRemoval(population_type&) =0;

			virtual void initializePopulation(population_type&) =0;

			virtual population_type crossOver(container_of_reference&) =0;

			virtual population_type mutate(container_of_reference&) =0;

		protected:
			population_type population;
		};
	}
}
