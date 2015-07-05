#pragma once
#include "../../Random/Random.h" // Darwin::Rand::uniform_distribution
#include <cstddef> // std::size_t
#include <array>
#include <vector>
#include <bitset>
#include <algorithm>

namespace Darwin
{
	namespace Rand
	{
		template<size_t N>
		class uniform_distribution<std::bitset<N>>
		{
		public:
			uniform_distribution()
			{
				dists.fill(std::bernoulli_distribution(0.5));
			}
			template<class Generator>
			std::bitset<N> operator()(Generator& gen)
			{
				std::bitset<N> ret;
				for (size_t position = 0; position < N; ++position)
				{
					ret[position] = dists[position](gen);
				}
				return ret;
			}
		private:
			std::array<std::bernoulli_distribution, N> dists;
		};

		template<>
		class uniform_distribution<std::vector<bool>>
		{
		public:
			uniform_distribution(size_t _size): size(_size)
			{
				dists.reserve(size);
				std::fill(dists.begin(), dists.end(), std::bernoulli_distribution(0.5));
			}
			template<class Generator>
			std::vector<bool> operator()(Generator& gen)
			{
				std::vector<bool> ret;
				ret.reserve(size);
				for(size_t position = 0; position < size; ++position)
				{
					ret.push_back(dists[position](gen));
				}
				return ret;
			}
		private:
			std::vector<std::bernoulli_distribution> dists;
			size_t const size;
		};
	}
}
