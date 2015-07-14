#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include "Random/Random.h"
#include "ISelection.h"

namespace Darwin
{
	template<class Individual, class Population= std::vector<Individual>, class Indices = std::vector<std::size_t>>
	class MixedSelection: public Interfaces::ISelection<Population, Indices>
	{
	public:
		explicit MixedSelection(double _threshold, double _score_proportion): threshold_ratio(_threshold), score_proportion(_score_proportion){}
		Indices operator()(Population& population)
		{

			size_t index_best = static_cast<size_t>(score_proportion*threshold_ratio*population.size());
			Indices indices_best(index_best);
			std::generate(std::begin(indices_best), std::end(indices_best), [&index_best]() { return --index_best;});
			index_best = static_cast<size_t>(score_proportion*threshold_ratio*population.size());
            size_t index_random = static_cast<size_t>(threshold_ratio*population.size()-index_best);
			Indices indices_random;
			std::vector<size_t> indices_random_temp(index_random);
			--index_best; // starting at right index
			std::generate(std::begin(indices_random_temp), std::end(indices_random_temp), [&index_best]() {return ++index_best;});
			size_t index_i; 
			for (size_t i = 0; i < index_random; i++){
				Darwin::Rand::uniform_distribution<size_t> dist(0,indices_random_temp.size()-1);
				index_i = dist(gen);
				indices_random.push_back(indices_random_temp[index_i]);
				indices_random_temp.erase(indices_random_temp.begin() + index_i);
			}
			Indices indices(indices_best);
			indices.insert( indices.begin(), indices_random.begin(), indices_random.end() );
			std::cout << " ... mixed selection done with size : " << indices.size() << std::endl;
			return indices;
		}
	private:
		std::mt19937 gen = std::mt19937(std::random_device()());
		double const threshold_ratio;
		double const score_proportion;
	};
}
