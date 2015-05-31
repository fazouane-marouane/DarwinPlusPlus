#pragma once

namespace Darwin
{
	namespace Interfaces
	{
		class IEvolutionaryConfig
		{
		public:
			virtual void breed() = 0;
			virtual bool goalReached() = 0;
		};
	}
}
