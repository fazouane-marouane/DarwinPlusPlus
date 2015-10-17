#pragma once
#include <DarwinConfig.h>
#include <vector>
#include <cstddef>

namespace Darwin
{
	class Permutation
	{
	public:
		explicit Permutation(size_t _size);
		Permutation(std::vector<size_t> const& permutation);
		Permutation(std::vector<size_t>&& permutation);
		Permutation(Permutation const& permutation) = default;
		Permutation(Permutation&& permutation) = default;
		Permutation& operator=(Permutation const& permutation) = default;
		Permutation& operator=(Permutation&& permutation) = default;

		std::vector<size_t> const& get() const;
		std::vector<size_t>& get();

		void transpose(size_t first, size_t second);
		void compose(Permutation const& other);

	private:
		size_t size;
		std::vector<size_t> _internal;
	};
}

// inline implementation
#if defined(DARWIN_INLINE)
#include <modules/permutation/permutation.cpp>
#endif