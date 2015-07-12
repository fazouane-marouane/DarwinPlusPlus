#pragma once

#include "permutationImpl.h"
#include "permutation_uniform_distribution.h"
#include <ostream>

inline std::ostream& operator<<(std::ostream& os, const Darwin::Permutation& permutation)
{
	os << "[ ";
	auto& vect = permutation.get();
	for (size_t itr = 0; itr < vect.size(); ++itr)
	{
		os << vect[itr];
		if (itr != vect.size() - 1)
			os << "->";
	}
	os << " ]";
	return os;
}

inline Darwin::Permutation operator*(Darwin::Permutation lhs, Darwin::Permutation const& rhs)
{
	lhs.compose(rhs);
	return lhs;
}
