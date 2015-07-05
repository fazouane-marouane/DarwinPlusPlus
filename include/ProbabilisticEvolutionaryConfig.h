#pragma once
#include <IEvolutionaryConfig.h>
#include <random>
#include "Random/Random.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace Darwin
{
	class NotImplementedException : public std::logic_error
	{
	public:
		NotImplementedException() : std::logic_error("Function not yet implemented.") {}
	};

	template<class GoalFunction, class Individual, class Population = std::vector<Individual>>
	class ProbabilisticEvolutionaryConfig: public Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>
	{
	private:
		using base = Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>;
		//using base::population_size;
	public:

		using typename base::population_type;
		using typename base::individuals_references;
		using typename base::individual_type;
		using base::population_size;
		using base::goalFunction;
		// sampling: multinomial distribution
		ProbabilisticEvolutionaryConfig(GoalFunction goal, size_t _population_size): base(goal, _population_size){}

		virtual individuals_references selectForCrossOver(population_type& population, std::string method = "uniform")
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			if (method == "uniform")
				return selectForCrossOver_uniform(population);
			else
				throw NotImplementedException();
		}

		virtual individuals_references selectForCrossOver_uniform(population_type& population)
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			Darwin::Rand::uniform_distribution<size_t> dis(0, population.size());
			size_t N = static_cast<size_t>(population_size*(0.3));
			auto rand = [&]() { return dis(gen); };
			std::vector<size_t> Values;

			while ( Values.size() < N )
			{
				Values.push_back(rand());
				std::sort( Values.begin(), Values.end() );
			    Values.erase( std::unique( Values.begin(), Values.end() ), Values.end() );
			}
			typename base::individuals_references list_individuals;
			for (auto& v : Values)
				list_individuals.push_back(population[v]);
			return list_individuals;
		}

		virtual individuals_references selectForMutation(population_type& population, std::string method = "uniform")
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			if (method == "uniform")
				return selectForMutation_uniform(population);
			else
				throw NotImplementedException();
		}

		virtual individuals_references selectForMutation_uniform(population_type& population)
		{
			// select over a population container
			size_t N = static_cast<size_t>(population_size*(0.3));
			Darwin::Rand::uniform_distribution<size_t> dis(1,N);
			auto rand = [&]() { return dis(gen); };
			std::vector<size_t> Values;

			while ( Values.size() < N )
			{
				Values.push_back(rand());
				sort( Values.begin(), Values.end() );
			    Values.erase( unique( Values.begin(), Values.end() ), Values.end() );
			}
			individuals_references list_individuals;
			for (auto& v : Values)
				list_individuals.push_back(population[v]);
			return list_individuals;
		}

		virtual std::vector<size_t> selectForRemoval(population_type& population, std::string method = "thresholding")
		{
			// Thresholding by default
			if (method == "thresholding")
				return selectForRemoval_thresholding(population);
			else 
				throw NotImplementedException();
		}

		virtual std::vector<size_t> selectForRemoval_thresholding(population_type& population){
			size_t n = 0;
			std::sort(population.begin(), population.end(), [this](individual_type& indiv1, individual_type& indiv2){return goalFunction(indiv1) < goalFunction(indiv2);});
			size_t N = static_cast<size_t>(population_size*(0.6));   // Keep only 60 % of the population
			std::vector<size_t> v(N);
			std::generate(v.rbegin(), v.rend(), [&n]{ return n++;});
            return v;
		}

		virtual void initializePopulation(population_type& population, std::string method = "uniform")
		{
			// distribution: uniform by default
			Darwin::Rand::uniform_distribution<Individual> dis;
			while ( population.size() < population_size )
			    population.push_back(dis(gen));
			population.push_back(dis(gen));
		}
	private:
		Rand::uniform_distribution<Individual> dist_crossOver;
		Rand::uniform_distribution<Individual> dist_mutation;
		Rand::uniform_distribution<Individual> dist_removal;
		Rand::uniform_distribution<Individual> dist_initialization;
		std::mt19937 gen = std::mt19937(std::random_device()());
	};
}
