#pragma once
#include <DarwinConfig.h>
#include <IEvolutionaryConfig.h>

namespace Darwin
{
	DARWIN_FUNCTION Interfaces::IEvolutionaryConfig& GeneticAlgorithmLoop(Interfaces::IEvolutionaryConfig&);
}

// inline implementation
#if defined(DARWIN_INLINE)
#include <GeneticAlgorithms.cpp>
#endif