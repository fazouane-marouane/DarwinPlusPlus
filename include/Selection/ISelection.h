#pragma once
#include <memory>

namespace Darwin
{
	namespace Interfaces
	{
		template<class Population, class Indices>
		class ISelection
		{
		public:
			using population_type = Population;
			using indices_type = Indices;
			virtual ~ISelection();
			virtual Indices operator()(Population& population) = 0;
		};

		template<class Population, class Indices>
		inline ISelection<Population, Indices>::~ISelection(){}

		template<class Population, class Indices>
		using SelectionType= std::unique_ptr<ISelection<Population, Indices>>;
	}

	template<class Option, class... Args>
	std::unique_ptr<Option> make_selection(Args&&... args)
	{
		return std::make_unique<Option>(std::forward<Args>(args)...);
	}
}
