#pragma once
#include <iterator>
#include <memory>

namespace Darwin
{
	namespace utility
	{
		template<class VectDest, class... VectSrc>
		inline void merge(VectDest& dest, VectSrc&&... src)
		{
			size_t size = dest.size();
			(void)(std::initializer_list<int>{(size +=src.size(),0) ...});
			dest.reserve(size);
			/*(void)(std::initializer_list<int>{
			(dest.insert(std::end(dest), std::begin(src), std::end(src)),0) ...});*/
			(void)(std::initializer_list<int>{
				(merge_helper(dest, std::forward<VectSrc>(src)), 0) ...});
		}

		template<class VectDest, class VectSrc>
		inline void merge_helper(VectDest& dest, VectSrc&& src)
		{
			for (auto&& v : src)
				dest.push_back(std::move(v));
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
