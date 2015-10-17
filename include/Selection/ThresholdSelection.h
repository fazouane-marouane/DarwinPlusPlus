#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include "ISelection.h"

namespace Darwin
{
	template<class Individual, class Population= std::vector<Individual>, class Indices = std::vector<std::size_t>>
	class ThresholdSelection: public Interfaces::ISelection<Population, Indices>
	{
	public:
		explicit ThresholdSelection(double _threshold): threshold_ratio(_threshold){}
		Indices operator()(Population& population)
		{
			size_t index = std::min(static_cast<size_t>(threshold_ratio*population.size()), population.size());
			Indices indices(index);
			
			std::generate(std::begin(indices), std::end(indices), [i = index]() mutable { return --i;});
			return indices;
		}
	private:
		double const threshold_ratio;
	};
}
