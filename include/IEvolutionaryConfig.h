#pragma once
#include <list> // std::list
#include <vector>
#include <functional> // std::reference_wrapper
#include <string>
#include <algorithm>
#include "Selection/ISelection.h"
#include "datastructures/algorithms.h"

namespace Darwin
{
	namespace Interfaces
	{
		class IEvolutionaryConfig
		{
		public:
			virtual ~IEvolutionaryConfig() {}
			virtual IEvolutionaryConfig& init() = 0;
			virtual IEvolutionaryConfig& breed() = 0;
			virtual bool goalReached() = 0;
		};

		template<class GoalFunction, class Individual, class Population>
		class IStandardEvolutionaryConfig: public IEvolutionaryConfig
		{
		public:
			using individual_type = Individual;
			using population_type = Population;
			using indices = std::vector<std::size_t>;

			IStandardEvolutionaryConfig(GoalFunction _goalFunction,
										SelectionType _selectForCrossOver = nullptr,
										SelectionType _selectForMutation = nullptr,
										SelectionType _selectForRemoval = nullptr) :
				goalFunction(_goalFunction),
				selectForCrossOver(_selectForCrossOver),
				selectForMutation(_selectForMutation),
				selectForRemoval(_selectForRemoval)
			{}

			// setters

			void setSelectionForCrossOver(SelectionType _selectForCrossOver)
			{
				selectForCrossOver = _selectForCrossOver;
			}

			void setSelectionForMutation(SelectionType _selectForMutation)
			{
				selectForMutation = _selectForMutation;
			}

			void setSelectionForRemoval(SelectionType _selectForRemoval)
			{
				selectForRemoval = _selectForRemoval;
			}

			// Genetic Algorithms

			virtual IEvolutionaryConfig& init()
			{
				initializePopulation(population);
				return *this;
			}

			virtual IEvolutionaryConfig& breed()
			{
				// Cross Over
				auto newIndividuals = crossOver(population, (*selectForCrossOver)(population));
				// Mutation
				auto mutants = mutate(population, (*selectForMutation)(population));
				// Merge these new individuals into the original population
				Darwin::utility::merge(population, newIndividuals, mutants);
				// sort
				std::sort(population.begin(), population.end(),
					[this](Individual& lhs, Individual& rhs)
					{
						return goalFunction(lhs) < goalFunction(rhs);
					});
				// Natural selection
				Darwin::utility::remove(population, (*selectForRemoval)(population));
				return *this;
			}

			virtual void initializePopulation(population_type&, std::string method = "uniform") = 0;

			virtual population_type crossOver(population_type population, indices const&) = 0;

			virtual population_type mutate(population_type population, indices const&) = 0;

			// Getters

			virtual population_type& getPopulation()
			{
				return population;
			}

			virtual population_type const& getPopulation() const
			{
				return population;
			}

			virtual GoalFunction& getGoalFunction()
			{
				return goalFunction;
			}

			virtual GoalFunction const& getGoalFunction() const
			{
				return goalFunction;
			}

		protected:
			population_type population;
			GoalFunction goalFunction;
			SelectionType selectForCrossOver;
			SelectionType selectForMutation;
			SelectionType selectForRemoval;
		};
        

	}
}
