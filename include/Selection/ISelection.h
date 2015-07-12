#pragma once

namespace Darwin
{
	namespace Interfaces
	{
		template<class Population, class Indices>
		class ISelection
		{
		public:
			virtual ~ISelection();
			virtual Indices operator()(Population& population) = 0;
		};

		template<class Population, class Indices>
		inline ISelection::~ISelection(){}
	}
}
