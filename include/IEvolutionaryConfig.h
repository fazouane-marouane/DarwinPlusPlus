#pragma once
#include <list> // std::list
#include <vector>
#include <cstddef>
#include <functional> // std::reference_wrapper
#include <string>
#include <algorithm>
#include <thread>
#include <omp.h>
#include "Selection/ISelection.h"
#include "Initialization/IInitialization.h"
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
			using selection_type = SelectionType<population_type,indices>;
			using initialization_type = InitializationType<population_type>;

			IStandardEvolutionaryConfig(GoalFunction _goalFunction,
										initialization_type&& _initializePopulation = nullptr,
										selection_type&& _selectForCrossOver = nullptr,
										selection_type&& _selectForMutation = nullptr,
										selection_type&& _selectForRemoval = nullptr) :
				goalFunction(_goalFunction),
				initializePopulation(std::move(_initializePopulation)),
				selectForCrossOver(std::move(_selectForCrossOver)),
				selectForMutation(std::move(_selectForMutation)),
				selectForRemoval(std::move(_selectForRemoval))
			{}

			// setters

			void setInitializer(initialization_type&& _initializePopulation)
			{
				initializePopulation = std::move(_initializePopulation);
			}

			void setSelectionForCrossOver(selection_type&& _selectForCrossOver)
			{
				selectForCrossOver = std::move(_selectForCrossOver);
			}

			void setSelectionForMutation(selection_type&& _selectForMutation)
			{
				selectForMutation = std::move(_selectForMutation);
			}

			void setSelectionForRemoval(selection_type&& _selectForRemoval)
			{
				selectForRemoval =std::move(_selectForRemoval);
			}

			// Genetic Algorithms

			virtual IEvolutionaryConfig& init()
			{
				(*initializePopulation)(population);
				return *this;
			}

			virtual IEvolutionaryConfig& breed()
			{
				// Cross Over
				population_type newIndividuals;// = crossOver(population, (*selectForCrossOver)(population));
				// Mutation
				population_type mutants;// = mutate(population, (*selectForMutation)(population));
				std::thread runner1([&newIndividuals, this](){ newIndividuals = crossOver(population, (*selectForCrossOver)(population)); });
				std::thread runner2([&mutants, this]() { mutants = mutate(population, (*selectForMutation)(population)); });
				runner1.join();
				runner2.join();
				// Merge these new individuals into the original population
				Darwin::utility::merge(population, newIndividuals, mutants);
				// sort
				#pragma omp parallel for
				for (auto itr = population.begin(); itr < population.end(); ++itr)
					(void)goalFunction(*itr);
				std::sort(population.begin(), population.end(),
					[this](Individual& lhs, Individual& rhs)
					{
						return goalFunction(lhs) < goalFunction(rhs);
					});
				// Natural selection
				Darwin::utility::remove(population, (*selectForRemoval)(population));
				return *this;
			}

			virtual population_type crossOver(population_type& population, indices const & parents)
			{
				std::vector<Individual> population_;
				#pragma omp parallel for
				for(auto itr = std::begin(parents); itr != std::end(parents); ++itr)
				{
					auto next_itr = std::next(itr);
					if (next_itr == std::end(parents))
						next_itr = std::begin(parents);
					population_.push_back(crossOver(population.at(*itr), population.at(*next_itr)));
				}
				return population_;
			}

			virtual population_type mutate(population_type& population, indices const & mutants)
			{
				Population population_;
				#pragma omp parallel for
				for (auto itr = mutants.begin(); itr < mutants.end(); ++itr)
					population_.push_back(mutate(population.at(*itr)));
				return population_;
			}

			virtual individual_type crossOver(individual_type const& lhs, individual_type const& rhs) = 0;

			virtual individual_type mutate(individual_type m) = 0;

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
			initialization_type initializePopulation;
			selection_type selectForCrossOver;
			selection_type selectForMutation;
			selection_type selectForRemoval;
		};
	}
}
