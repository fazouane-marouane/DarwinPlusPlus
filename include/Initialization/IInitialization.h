#pragma once
#include <cstddef>
#include <memory>

namespace Darwin
{
	namespace Interfaces
	{
		template<class Population>
		class IInitialization
		{
		public:
			using population_type = Population;
			virtual ~IInitialization();
			virtual size_t operator()(Population& population) = 0;
		};

		template<class Population>
		inline IInitialization<Population>::~IInitialization(){}

		template<class Population>
		using InitializationType= std::unique_ptr<IInitialization<Population>>;
	}

	template<class Initialization, class... Args>
	std::unique_ptr<Initialization> make_initialization(Args&&... args)
	{
		return std::make_unique<Initialization>(std::forward<Args>(args)...);
	}
}
