#pragma once
#include <algorithm>
#include <iterator>
#include "ISelection.h"

namespace Darwin
{
	template<class Individual, class Population, class Indices>
	class ThresholdSelection: public Interfaces::ISelection<Population, GoalFunction, Indices>
	{
	public:
		explicit ThresholdSelection(size_t _threshold): threshold(_threshold){}
		Indices operator()(Population& /*population*/)
		{
			Indices indices(threshold);
			size_t index = 0U;
			std::generate(std::rbegin(indices), std::rend(indices), [&index]() { return index++;});
			return indices;
		}
	private:
		size_t const threshold;
	};
}
