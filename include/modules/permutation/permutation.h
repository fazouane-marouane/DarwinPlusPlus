#pragma once

#include "permutationImpl.h"
#include "permutation_uniform_distribution.h"

inline Darwin::Permutation operator*(Darwin::Permutation lhs, Darwin::Permutation const& rhs)
{
	lhs.compose(rhs);
	return lhs;
}
