#pragma once
#include <DarwinConfig.h>
#ifndef DARWIN_INLINE
#include <modules/permutation/permutation.h>
#endif
#include <algorithm>
#include <iterator>
#include <cassert>
#if defined(_OPENMP)
#include <omp.h>
#endif

namespace Darwin
{
	Permutation::Permutation(size_t _size) : size(_size), _internal(_size)
	{
		size_t n = 0U;
		std::generate(std::begin(_internal), std::end(_internal), [&n]{ return n++; });
	}

	Permutation::Permutation(std::vector<size_t> const& permutation): size(permutation.size()), _internal(permutation)
	{
		/*assert(size > 0);
		std::vector<size_t> test(size);
		size_t n = 0U;
		std::generate(std::begin(test), std::end(test), [&n] { return n++; });
		assert(std::is_permutation(_internal.begin(), _internal.end(),test.begin(), test.end() ));*/
	}

	Permutation::Permutation(std::vector<size_t>&& permutation) : size(permutation.size()), _internal(permutation)
	{
		/*assert(size > 0);
		std::vector<size_t> test(size);
		size_t n = 0U;
		std::generate(std::begin(test), std::end(test), [&n] { return n++; });
		assert(std::is_permutation(_internal.begin(), _internal.end(), test.begin(), test.end()));*/
	}

	std::vector<size_t> const& Permutation::get() const
	{
		return _internal;
	}

	std::vector<size_t>& Permutation::get()
	{
		return _internal;
	}

	void Permutation::invert()
	{
		std::vector<size_t> result(_internal.size());
		#pragma omp parallel for
		for (int itr = 0; itr < size; ++itr)
		{
			result[_internal[itr]] = itr;
		}
		using std::swap;
		swap(result, _internal);
	}

	void Permutation::reverse()
	{
		std::vector<size_t> result(_internal.size());
		#pragma omp parallel for
		for (int itr = 0; itr < size; ++itr)
		{
			result[size-1-itr] = _internal[itr];
		}
		using std::swap;
		swap(result, _internal);
	}

	void Permutation::transpose(size_t first, size_t second)
	{
		/*assert(first < size && second < size);*/
		using std::swap;
		swap(_internal[first], _internal[second]);
	}

	void Permutation::reverseAllBetween(size_t first, size_t last)
	{
		//assert(first < size && second < size);
		if (last < first)
			return reverseAllBetween(last, first);
		if (last == first)
			return;
		using std::swap;
		#pragma omp parallel for
		for (int itr = 0; itr <= (last-first)/2; ++itr)
			swap(_internal[first+itr], _internal[last-itr]);
	}

	void Permutation::compose(Permutation const& other)
	{
		std::vector<size_t> result(_internal);
		#pragma omp parallel for
		for (int itr = 0; itr < size; ++itr)
		{
			result[itr] = _internal[other._internal[itr]];
		}
		using std::swap;
		swap(result, _internal);
	}
}
