#pragma once

#include "bitstring_uniform_distribution.h"
#include <cassert>

inline std::ostream& operator<<(std::ostream& os, std::vector<bool> const& vec)
{
	for (auto el : vec)
		os << el;
	return os;
}

inline std::vector<bool> operator&(std::vector<bool> lhs, std::vector<bool> const& rhs)
{
	assert(lhs.size() <= rhs.size());
	for (size_t pos = 0; pos < lhs.size(); ++pos)
		lhs[pos] = lhs[pos] && rhs[pos];
	return lhs;
}

inline std::vector<bool> operator|(std::vector<bool> lhs, std::vector<bool> const& rhs)
{
	assert(lhs.size() <= rhs.size());
	for (size_t pos = 0; pos < lhs.size(); ++pos)
		lhs[pos] = lhs[pos] || rhs[pos];
	return lhs;
}
