#pragma
#include <Random/Random.h> // Darwin::Rand::uniform_distribution
#include <cstddef> // std::size_t
#include <array> // std::array
#include <cassert> // std::assert
#include <string> // std::string

template<size_t N>
class length_equals;

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
	}
}
