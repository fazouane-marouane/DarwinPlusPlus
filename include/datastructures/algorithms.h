#pragma once
#include <iterator>

namespace Darwin
{
	namespace utility
	{
		template<class VectDest, class... VectSrc>
		void merge(VectDest& dest, VectSrc&&... src)
		{
			(void)(std::initializer_list<int>{
			(dest.insert(std::end(dest), std::begin(src), std::end(src)),0) ...});
		}

		template<class VectDest, class Indx>
		void remove(VectDest& dest, Indx toBeRemoved)
		{
			// check that toBeRemoved is sorted
			for(auto v: toBeRemoved)
				dest.erase(dest.begin()+v);
		}
	}
}
