#include <Random/Random.h>
#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include "extend_random.h"
#include <iostream>
#include <functional>

using Individual = std::bitset<3>;
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>
{
	using base = Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>;
	public :
		using base::base;

	virtual std::vector<Individual> crossOver(std::list<std::reference_wrapper<Individual>> const &)
	{
		std::vector<Individual> population_;
		return population_;
	}

	virtual std::vector<Individual> mutate(std::list<std::reference_wrapper<Individual>> const &)
	{
		std::vector<Individual> population_;
		return population_;
	}
	virtual bool goalReached()
	{
		return false;
	}
};

int main()
{
	auto goalFunction = std::function<float(Individual)>([](std::bitset<3> individual)
	{
		float s = 0;
		for(size_t it = 0; it < 3; ++it)
		{
			s += individual[it];
		}
		return s;
	});
	//IStandardEvolutionaryConfig<std::function<float(Individual)>, Individual, std::vector<Individual> > standardEvolutionary(goalFunction);
	testEvolutionaryConfig t(goalFunction);
    t.init();
	t.breed();
    return 0;
}
