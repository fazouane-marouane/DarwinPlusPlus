#pragma once
#include <Random/Random.h> // Darwin::Rand::uniform_distribution
#include <cstddef> // std::size_t
#include <array> // std::array
#include <bitset>

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
				for (int position = 0; position < N; ++position)
				{
					ret[position] = dists[position](gen);
				}
				return ret;
			}
			std::array<std::bernoulli_distribution, N> dists;
		};
	}
}
