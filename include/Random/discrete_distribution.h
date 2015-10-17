 #pragma once
#include "Random.h"
#include <limits>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>

namespace Darwin
{
	namespace Rand
	{
		class cumulated_sums_t
		{};
		class probabilities_t
		{};
		class positive_values_t
		{};

		template<class _Double>
		class discrete_distribution
		{
		public:
			discrete_distribution() : uniform_variable(0, 1)
			{}

			discrete_distribution(std::vector<_Double> const& vect, positive_values_t) : uniform_variable(0, 1)
			{
				init_with_positive_values(vect);
			}

			discrete_distribution(std::vector<_Double> const& vect, probabilities_t) : uniform_variable(0, 1)
			{
				init(vect);
			}

			discrete_distribution(std::vector<_Double> const& vect, cumulated_sums_t) : uniform_variable(0, 1)
			{
				init_with_cumulated_sums(vect);
			}

			void init(std::vector<_Double> const& vect_probabilities)
			{
				_initialized = true;
				upper_limits.erase(std::begin(upper_limits), std::end(upper_limits));
				_Double cumsum = 0.0;
				for (auto p : vect_probabilities)
				{
					cumsum += p;
					upper_limits.push_back(cumsum);
				}
				assert(std::abs(cumsum - 1.0) < std::numeric_limits<_Double>::epsilon());
			}

			void init_with_positive_values(std::vector<_Double> vect)
			{
				_Double sum = 0;
				for (auto v : vect)
				{
					assert(v >= 0.0);
					sum += vect;
				}
				assert(sum > 0.0);
				for (auto& v : vect)
					v /= sum;
				init(vect);
			}

			void init_with_cumulated_sums(std::vector<_Double> const& vect_sums)
			{
				for (size_t itr = 1; itr < vect_sums.size(); ++itr)
				{
					assert(vect_sums[itr-1] <= vect_sums[itr]);
				}
				assert(std::abs(vect_sums.back() - 1.0) < std::numeric_limits<_Double>::epsilon());
				_initialized = true;
				upper_limits = vect_sums;
			}

			template<class Generator>
			size_t operator()(Generator& gen)
			{
				assert(_initialized);
				auto itr = std::upper_bound(std::begin(upper_limits), std::end(upper_limits), uniform_variable(gen));
				return static_cast<size_t>(itr - std::begin(upper_limits));
			}
		private:
			std::vector<double> upper_limits;
			uniform_distribution<double> uniform_variable;
			bool _initialized = false;
		};

		template<class _Double>
		discrete_distribution<_Double> make_distribution(std::vector<_Double> probabilities)
		{
			return discrete_distribution<_Double>(probabilities, probabilities_t{});
		}

		template<class _Double>
		discrete_distribution<_Double> make_distribution(std::initializer_list<_Double> probabilities)
		{
			return discrete_distribution<_Double>(probabilities, probabilities_t{});
		}

		template<class _Double>
		discrete_distribution<_Double> make_distribution_from_positive_values(std::vector<_Double> values)
		{
			return discrete_distribution<_Double>(values, positive_values_t{});
		}

		template<class _Double>
		discrete_distribution<_Double> make_distribution_from_positive_values(std::initializer_list<_Double> values)
		{
			return discrete_distribution<_Double>(values, positive_values_t{});
		}

		template<class _Double>
		discrete_distribution<_Double> make_distribution_from_cumsum(std::vector<_Double> cumsum)
		{
			return discrete_distribution<_Double>(cumsum, cumulated_sums_t{});
		}

		template<class _Double>
		discrete_distribution<_Double> make_distribution_from_cumsum(std::initializer_list<_Double> cumsum)
		{
			return discrete_distribution<_Double>(cumsum, cumulated_sums_t{});
		}
	}
}
