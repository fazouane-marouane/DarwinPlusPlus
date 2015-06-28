#pragma once
#include <Random/Random.h> // Darwin::Rand::uniform_distribution
#include <Random/discrete_distribution.h>
#include <cstddef> // std::size_t
#include <array> // std::array
#include <cassert> // std::assert
#include <string> // std::string
#include <algorithm>
#include <iterator>

template<size_t N>
class length_equals;

template<size_t N>
class length_less_than;


namespace Darwin
{
	namespace Rand
	{
		template<size_t N>
		class uniform_distribution<std::string, length_equals<N>>
		{
		public:
			uniform_distribution(char min = 'a', char max = 'z')
			{
				assert(min <= max);
				dists.fill(uniform_distribution<int>(min, max));
			}
			template<class Generator>
			std::string operator()(Generator& gen)
			{
				std::string ret;
				for (int position = 0; position < N; ++position)
				{
					ret += static_cast<char>(dists[position](gen));
				}
				return ret;
			}
			std::array<uniform_distribution<int>, N> dists;
		};

		template<size_t N>
		class uniform_distribution<std::string, length_less_than<N>>
		{
		public:
			uniform_distribution(char min = 'a', char max = 'z'): length_dist()
			{
				assert(min <= max);
				dists.fill(uniform_distribution<int>(min, max));
				double alpha_size = max - min + 1;
				double maxi = std::pow(alpha_size, N) - 1.0;
				double r = alpha_size;
				std::vector<double> cumsum;
				for (size_t itr = 0; itr < N; ++itr)
				{
					cumsum.push_back((r - 1) / maxi);
					r *= alpha_size;
				}
				length_dist.init_with_cumulated_sums(cumsum);
			}
			template<class Generator>
			std::string operator()(Generator& gen)
			{
				size_t length = length_dist(gen)+1;
				std::string ret;
				for (size_t position = 0; position < length; ++position)
				{
					ret += static_cast<char>(dists[position](gen));
				}
				return ret;
			}
		private:
			std::array<uniform_distribution<int>, N> dists;
			discrete_distribution<double> length_dist;
		};

	}

	namespace utility
	{
		class string_length_then_alphabetical_ordering
		{
		public:
			bool operator()(std::string const& lhs, std::string const& rhs) const
			{
				if (lhs.length() == rhs.length())
					return lhs < rhs;
				else
					return lhs.length() < rhs.length();
			}
		};
	}
}
