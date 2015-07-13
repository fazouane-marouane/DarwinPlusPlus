#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include <cstddef>
#include "ISelection.h"
#include "../Random/Random.h"

namespace Darwin
{
	template<class Individual, class Population = std::vector<Individual>, class Indices= std::vector<size_t>>
	class UniformSelection: public Interfaces::ISelection<Population, Indices>
	{
	public:
		explicit UniformSelection(double _threshold_ratio, size_t _population_size): threshold_ratio(_threshold_ratio), dist(0, _population_size -1){}

		Indices operator()(Population& population)
		{
			size_t threshold = static_cast<size_t>(threshold_ratio*population.size());
			auto rand = [&]() { return dist(gen); };
			std::vector<size_t> indices;

			while ( indices.size() < threshold )
			{
				indices.push_back(rand());
				sort( std::begin(indices), std::end(indices) );
				indices.erase( unique( std::begin(indices), std::end(indices) ), std::end(indices) );
			}
			return indices;
		}
	private:
		size_t const threshold_ratio;
		Rand::uniform_distribution<size_t> dist;
		std::mt19937 gen = std::mt19937(std::random_device()());
	};
}
