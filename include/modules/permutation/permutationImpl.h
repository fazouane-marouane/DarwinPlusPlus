#pragma once
#include <vector>
#include <cstddef>

namespace Darwin
{
	class Permutation
	{
	public:
		explicit Permutation(size_t _size);
		Permutation(std::vector<size_t> permutation);
		std::vector<size_t> const& get() const;
		std::vector<size_t>& get();

		void transpose(size_t first, size_t second);
		void compose(Permutation const& other);

	private:
		size_t size;
		std::vector<size_t> _internal;
	};
}
