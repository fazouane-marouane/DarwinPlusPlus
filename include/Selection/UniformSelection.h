#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <cstddef>
#include "ISelection.h"
#include "../Random/Random.h"

namespace Darwin
{
	template<class Individual, class Population = std::vector<Individual>, class Indices= std::vector<size_t>>
	class UniformSelection: public Interfaces::ISelection<Population, Indices>
	{
	public:
		explicit UniformSelection(double _threshold_ratio): threshold_ratio(_threshold_ratio), dist(0, 1){}

		Indices operator()(Population& population)
		{
			size_t threshold = std::min(static_cast<size_t>(threshold_ratio*population.size()), population.size());
			std::vector<size_t> indices;
			indices.reserve(threshold);
			std::vector<size_t> v(population.size());
			auto rand = [&]() { return std::floor(dist(gen)*v.size()); };

			size_t index = 0U;
			std::generate(std::begin(v), std::end(v), [&index] {return index++; });

			while( indices.size() < threshold)
			{
				size_t i = rand();
				indices.push_back(v[i]);
				v.erase(v.begin() + i);
			}
			std::sort(indices.begin(), indices.end(), std::greater<size_t>());
			return indices;
		}
	private:
		double const threshold_ratio;
		Rand::uniform_distribution<double> dist;
		std::mt19937 gen = std::mt19937(std::random_device()());
	};
}
