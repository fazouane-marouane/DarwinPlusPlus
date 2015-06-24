#pragma
#include <Random/Random.h> // Darwin::Rand::uniform_distribution
#include <cstddef> // std::size_t
#include <array> // std::array
#include <cassert> // std::assert
#include <string> // std::string

namespace Darwin
{
	namespace Rand
	{
		template<>
		class uniform_distribution<std::string>
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
				for (int position = 0; position < 5; ++position)
				{
					ret += static_cast<char>(dists[position](gen));
				}
				return ret;
			}
			std::array<uniform_distribution<int>, 5> dists;
		};
	}
}
