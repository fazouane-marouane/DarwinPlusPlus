#pragma once
#include <algorithm>
#include <iterator>
#include "ISelection.h"
#include "../Random/Random.h"

namespace Darwin
{
	template<class Distribution, class Individual, class Population, class Indices>
	class UniformSelection: public Interfaces::ISelection<Population, GoalFunction, Indices>
	{
	public:
		explicit UniformSelection(size_t _threshold): threshold(_threshold), dist(0,_threshold-1){}

		Indices operator()(Population& population)
		{
			auto rand = [&]() { return dist(gen); };
			std::vector<size_t> Indices;

			while ( Indices.size() < threshold )
			{
				Indices.push_back(rand());
				sort( Indices.begin(), Indices.end() );
				Indices.erase( unique( Indices.begin(), Indices.end() ), Indices.end() );
			}
			return Indices;
		}
	private:
		size_t const threshold;
		Distribution dist;
		std::mt19937 gen = std::mt19937(std::random_device()());
	};
}
