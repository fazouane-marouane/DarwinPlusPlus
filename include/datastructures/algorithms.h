#pragma once
#include <iterator>
#include <memory>
#if defined(DARWIN_OPENMP)
#include <omp.h>
#endif

namespace Darwin
{
	namespace utility
	{
		template<class VectDest, class VectSrc>
		inline void merge_helper(VectDest& dest, VectSrc&& src)
		{
			for (auto&& v : src)
				dest.push_back(std::move(v));
		}

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

		template<class VectDest, class Indx>
		inline void remove(VectDest& dest, Indx&& toBeRemoved)
		{
			VectDest tmp;
			tmp.reserve(dest.size() - toBeRemoved.size());
			const size_t N = dest.size();
			#pragma omp parallel
			{
				VectDest tmp_private;
				#pragma omp for
				for (int i = 0; i < N; i++)
				{
					bool removeIt = false;
					for (auto v : toBeRemoved)
						if (v == i) {
							removeIt = true;
							break;
						}
					if (!removeIt)
						tmp_private.push_back(std::move(dest[i]));
				}
				#pragma omp critical
				{
					Darwin::utility::merge(tmp, tmp_private);
				}
			}
			
			dest = std::move(tmp);
			// check that toBeRemoved is sorted
			/*
			for(auto v: toBeRemoved)
				dest.erase(dest.begin()+v);*/
		}
	}
}
