#pragma once
#include "IInitialization.h"
#include <vector>
#include "../Random/Random.h"

namespace Darwin
{
	template<class Population, class RandomDistribution>
	class RandomInitialization: public Interfaces::IInitialization<Population>
	{
	public:
		template<class... DistArgs>
		RandomInitialization(std::size_t _target_population_size, DistArgs&&... dist_args):
			target_population_size(_target_population_size),
			dist(std::forward<DistArgs>(dist_args)...)
		{}

		std::size_t operator()(Population& population)
		{
			while ( population.size() <= target_population_size )
				population.push_back(dist(gen));
			return target_population_size;
		}

	protected:
		std::size_t target_population_size;
	private:
		RandomDistribution dist;
		std::random_device rd;
		std::mt19937 gen = std::mt19937(rd());
	};

	template<class Individual, class Population = std::vector<Individual>>
	using UniformInitialization = RandomInitialization<Population, Rand::uniform_distribution<Individual>>;
}
