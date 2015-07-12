#include "modules/permutation/permutationImpl.h"
#include <algorithm>
#include <iterator>
#include <cassert>

namespace Darwin
{
	inline Permutation::Permutation(size_t _size) : size(_size), _internal(_size)
	{
		size_t n = 0U;
		std::generate(std::begin(_internal), std::end(_internal), [&n]{ return n++; });
	}

	inline Permutation::Permutation(std::vector<size_t> permutation): size(permutation.size()), _internal(permutation)
	{
		assert(size >= 0);
		std::vector<size_t> test(size);
		size_t n = 0U;
		std::generate(std::begin(test), std::end(test), [&n] { return n++; });
		assert(std::is_permutation(_internal.begin(), _internal.end(),test.begin(), test.end() ));
	}

	inline std::vector<size_t> const& Permutation::get() const
	{
		return _internal;
	}

	inline std::vector<size_t>& Permutation::get()
	{
		return _internal;
	}

	inline void Permutation::transpose(size_t first, size_t second)
	{
		assert(first < size && second < size);
		using std::swap;
		swap(_internal[first], _internal[second]);
	}

	void Permutation::compose(Permutation const& other)
	{
		std::vector<size_t> result(_internal);
		for (size_t itr = 0; itr < size; ++itr)
		{
			result[itr] = _internal[other._internal[itr]];
		}
		using std::swap;
		swap(result, _internal);
	}
}
