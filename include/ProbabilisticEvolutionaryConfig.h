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
		using base = Interfaces::IStandardEvolutionaryConfig<Individual, Population>;
	public:
		// sampling: multinomial distribution
		using base::base;

		virtual typename base::individuals_references selectForCrossOver(typename base::population_type& population, method = 'uniform')
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
            return selectForCrossOver_uniform(population, method);
		}

		virtual typename base::individuals_references selectForCrossOver_uniform(typename base::population_type& population, method = 'uniform')
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			std::random_device rd;
			std::mt19937 gen(rd());
			Darwin::Rand::uniform_distribution<int> dis_size(int(population.size()/3),int((2/3)*population.size()));
			N = dis_size(gen);
			Darwin::Rand::uniform_distribution<int> dis(1,N);
			auto rand = [&]() { return dis(gen); };
			std::vector<int> Values;

			while ( Values.size() < N )
			{
				Values.push_back(rand());
				sort( Values.begin(), Values.end() );
			    Values.erase( unique( Values.begin(), Values.end() ), Values.end() );
			}
			base::individuals_references list_individuals;
			std::copy( Values.begin(), Values.end(), std::back_inserter( list_individuals ) );  
			return list_individuals;          
		}

		virtual typename base::individuals_references selectForMutation(typename base::population_type& population, method = 'uniform')
		{
			// multinomial by default
			// other methods: Tournament, SCX
			// select over a population container
			return selectForMutation_uniform(population, method);
		}

		virtual typename base::individuals_references selectForMutation_uniform(typename base::population_type& population, method = 'multinomial')
		{
			// uniform by default
			// other methods: Tournament, SCX
			// select over a population container
			std::random_device rd;
			std::mt19937 gen(rd());
			Darwin::Rand::uniform_distribution<int> dis_size(int(population.size()/3),int((2/3)*population.size()));
			N = dis_size(gen);
			Darwin::Rand::uniform_distribution<int> dis(1,N);
	        auto rand = [&]() { return dis(gen); };
	        std::vector<int> Values;

	        while ( Values.size() < N )
	        {
	        	Values.push_back(rand());
	        	sort( Values.begin(), Values.end() );
                Values.erase( unique( Values.begin(), Values.end() ), Values.end() );
	        }
	        base::individuals_references list_individuals;
			std::copy( Values.begin(), Values.end(), std::back_inserter( list_individuals ) );  
			return list_individuals;  

		}

		virtual typename base::individuals_references selectForRemoval(typename base::population_type& population, method = 'multinomial')
		{
			// multinomial method by default
		}

		virtual void initializePopulation(typename base::population_type& population, method = 'uniform')
		{
			// distribution: uniform by default
			Darwin::Rand::uniform_distribution<Individual> dis;
			std::random_device rd;
			std::mt19937 gen(rd());
			population = dis(gen);
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
