#pragma once
#include <list> // std::list
#include <vector>
#include <functional> // std::reference_wrapper
#include <string>
#include "datastructures/algorithms.h"

namespace Darwin
{
	namespace Interfaces
	{
		class IEvolutionaryConfig
		{
		public:
			virtual ~IEvolutionaryConfig();
			virtual IEvolutionaryConfig& init() = 0;
			virtual IEvolutionaryConfig& breed() = 0;
			virtual bool goalReached() = 0;
		};

		IEvolutionaryConfig::~IEvolutionaryConfig() {}

		template<class GoalFunction, class Individual, class Population>
		class IStandardEvolutionaryConfig: public IEvolutionaryConfig
		{
		public:
			using individual_type = Individual;
			using population_type = Population;
			using individuals_references = std::list<std::reference_wrapper<Individual>>; // TODO: lame naming

			IStandardEvolutionaryConfig(GoalFunction _goalFunction, size_t _population_size) : goalFunction(_goalFunction), population_size(_population_size){}

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
				// Merge these new individuals into the original population
				Darwin::utility::merge(population, newIndividuals, mutants);
				// Natural selection
				Darwin::utility::remove(population, selectForRemoval(population));
				return *this;
			}

			virtual individuals_references selectForCrossOver(population_type&, std::string = "uniform") = 0;

			virtual individuals_references selectForMutation(population_type&, std::string = "uniform") = 0;

			virtual std::vector<size_t> selectForRemoval(population_type&, std::string = "thresholding") = 0;

			virtual void initializePopulation(population_type&, std::string method = "uniform") = 0;

			virtual population_type crossOver(individuals_references const&) = 0;

			virtual population_type mutate(individuals_references const&) = 0;

		protected:
			population_type population;
			size_t population_size;
			GoalFunction goalFunction;
		};
        

	}
}
