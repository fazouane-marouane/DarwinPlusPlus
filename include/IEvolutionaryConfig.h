#pragma once
#include <list> // std::list
#include <vector>
#include <cstddef>
#include <functional> // std::reference_wrapper
#include <string>
#include <algorithm>
#include <map>
#include <thread>
#include <chrono>
#if defined(_OPENMP)
#include <omp.h>
#endif

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
				auto startCompute = std::chrono::steady_clock::now();
				// Cross Over
				population_type newIndividuals;
				// Mutation
				population_type mutants;
				{
					std::thread tasks[] = {
						std::thread([this, &newIndividuals]() { newIndividuals = crossOver(population, (*selectForCrossOver)(population)); }),
						std::thread([this, &mutants]() { mutants = mutate(population, (*selectForMutation)(population)); })
					};
					for (auto& t : tasks)
						t.join();
				}
				auto endCompute = std::chrono::steady_clock::now();
				// Merge these new individuals into the original population
				Darwin::utility::merge(population, std::move(newIndividuals), std::move(mutants));
				auto endMerge = std::chrono::steady_clock::now();
				// sort
				//#pragma omp parallel
				{
					size_t N = population.size();
					for (int itr = 0; itr < N; itr++)
						(void)goalFunction(population.at(itr));
				}
				auto endComputeGoalFunc = std::chrono::steady_clock::now();
				std::sort(population.begin(), population.end(),
					[this](Individual& lhs, Individual& rhs)
					{
						return goalFunction(lhs) < goalFunction(rhs);
					});
				auto endSort = std::chrono::steady_clock::now();
				// Natural selection
				auto toBeRemoved = (*selectForRemoval)(population);
				auto endSelectRemove = std::chrono::steady_clock::now();
				Darwin::utility::remove(population, std::move(toBeRemoved));
				auto endBreed = std::chrono::steady_clock::now();
				perfs["Compute"] += endCompute - startCompute;
				perfs["Merge"] += endMerge - endCompute;
				perfs["GoalFunc"] += endComputeGoalFunc - endMerge;
				perfs["Sort"] += endSort - endComputeGoalFunc;
				perfs["SelectRemove"] += endSelectRemove - endSort;
				perfs["Remove"] += endBreed - endSelectRemove;
				return *this;
			}

			virtual population_type crossOver(population_type& population, indices const & parents)
			{
				std::vector<Individual> population_;
				int N = parents.size();
				population_.reserve(N);
				#pragma omp parallel
				{
					std::vector<Individual> population_private;
					#pragma omp for
					for (auto itr = 0; itr < N; itr++)
					{
						auto next_itr = (itr + 1) % N;
						population_private.push_back(crossOver(population.at(parents.at(itr)), population.at(parents.at(next_itr))));
					}
					#pragma omp critical
					{
						Darwin::utility::merge(population_, population_private);
					}
				}

				
				return population_;
			}

			virtual population_type mutate(population_type& population, indices const & mutants)
			{
				Population population_;
				size_t N = mutants.size();
				population_.reserve(N);
				#pragma omp parallel
				{
					std::vector<Individual> population_private;
					#pragma omp for
					for (auto itr = 0; itr < N; itr++)
					{
						population_private.push_back(mutate(population.at(mutants.at(itr))));
					}
					#pragma omp critical
					{
						Darwin::utility::merge(population_, population_private);
					}
				}
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

			virtual std::map<std::string, std::chrono::duration<double, std::nano>> const& getPerfs() const
			{
				return perfs;
			}

		protected:
			population_type population;
			GoalFunction goalFunction;
			initialization_type initializePopulation;
			selection_type selectForCrossOver;
			selection_type selectForMutation;
			selection_type selectForRemoval;

			std::map<std::string, std::chrono::duration<double, std::nano>> perfs;
		};
	}
}
