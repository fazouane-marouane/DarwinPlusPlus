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
		using base = Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>;
	public:
		// sampling: multinomial distribution
		ProbabilisticEvolutionaryConfig(GoalFunction goal, size_t _sizePopulationInit): base(goal), sizePopulationInit(_sizePopulationInit)
		{}

		virtual typename base::individuals_references selectForCrossOver(typename base::population_type& population, std::string method = "uniform")
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			if (method == "uniform")
				return selectForCrossOver_uniform(population);
			else
				throw NotImplementedException();
		}

		virtual typename base::individuals_references selectForCrossOver_uniform(typename base::population_type& population)
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			Darwin::Rand::uniform_distribution<size_t> dis_size(population.size()/3, static_cast<size_t>((2.0/3)*population.size()));
			// size_t N = dis_size(gen);
			size_t N = (int)population_size*(0.3);
			Darwin::Rand::uniform_distribution<int> dis(1,N);
			auto rand = [&]() { return dis(gen); };
			std::vector<int> Values;

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

		virtual typename base::individuals_references selectForMutation(typename base::population_type& population, std::string method = "uniform")
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			if (method == "uniform")
				return selectForMutation_uniform(population);
			else
				throw NotImplementedException();
		}

		virtual typename base::individuals_references selectForMutation_uniform(typename base::population_type& population)
		{
			// select over a population container
			Darwin::Rand::uniform_distribution<size_t> dis_size(population.size()/3, static_cast<size_t>((2.0/3)*population.size()));
			//size_t N = dis_size(gen);
			size_t N = (int)population_size*(0.3);
			Darwin::Rand::uniform_distribution<int> dis(1,N);
			auto rand = [&]() { return dis(gen); };
			std::vector<int> Values;

			while ( Values.size() < N )
			{
				Values.push_back(rand());
				sort( Values.begin(), Values.end() );
			    Values.erase( unique( Values.begin(), Values.end() ), Values.end() );
			}
			typename base::individuals_references list_individuals;
			for (auto& v : Values)
				list_individuals.push_back(population[v]);
			return list_individuals;
		}

		virtual typename std::vector<size_t> selectForRemoval(typename base::population_type& population, std::string method = "thresholding")
		{
			// Thresholding by default
			if method == "thresholding"
				return selectForRemoval_thresholding(population);
			else 
				throw NotImplementedException();
		}

		virtual typename std::vector<size_t> selectForRemoval_thresholding(typename base::population_type& population){
			int n = 0;
			population_triee = std::sort(population.begin(), population.end(), [individual_type indiv1, individual_type indiv2]{return goalFunction(indiv1) < goalFunction(indiv2);});
			size_t N = (int)population_size*(0.6);   // Keep only 60 % of the population
			std::vector<int> v(N);
            return std::generate(v.begin(), v.end(), [&n]{ return n++;});
		}

		virtual void initializePopulation(typename base::population_type& population, std::string method = "uniform")
		{
			// distribution: uniform by default
			Darwin::Rand::uniform_distribution<Individual> dis;
			while ( population.size() < sizePopulationInit )
			    population.push_back(dis(gen));
			population.push_back(dis(gen));
		}
	private:
		Rand::uniform_distribution<Individual> dist_crossOver;
		Rand::uniform_distribution<Individual> dist_mutation;
		Rand::uniform_distribution<Individual> dist_removal;
		Rand::uniform_distribution<Individual> dist_initialization;
		std::mt19937 gen = std::mt19937(std::random_device()());
		size_t sizePopulationInit;
	};
}
