#pragma once
#include "../../Random/Random.h" // Darwin::Rand::uniform_distribution
#include <cstddef> // std::size_t
#include <vector>
#include "permutationImpl.h"
#include <algorithm>

namespace Darwin
{
	namespace Rand
	{
		template<>
		class uniform_distribution<Permutation>
		{
		public:
			uniform_distribution(size_t _size): size(_size), dists(_size), simple_permutation(_size)
			{
				for (size_t itr = size - 1; itr >= 0; --itr)
					dists[itr] = uniform_distribution<size_t>(0, itr);
				size_t n = 0U;
				std::generate(std::begin(simple_permutation), std::end(simple_permutation), [&n] { return n++; });
			}

			template<class Generator>
			Permutation operator()(Generator& gen)
			{
				std::vector<size_t> result;
				ret.reserve(size);
				auto permutation = simple_permutation;
				for(size_t position = 0; position < size; ++position)
				{
					auto pos = dists[position](gen);
					auto itr = permutation.begin() + pos;
					ret.push_back(*itr);
					permutation.erase(itr);
				}
				return ret;
			}
		private:
			size_t const size;
			std::vector<uniform_distribution<size_t>> dists;
			std::vector<size_t> simple_permutation;
		};
	}
}
