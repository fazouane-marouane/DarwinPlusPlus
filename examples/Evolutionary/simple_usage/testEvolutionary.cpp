// std
#include <iostream>
#include <functional>
// darwin
#include <modules/bitstring/bitstring.h>
#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>

using Individual = std::bitset<3>;

template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>
{
	using base = Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>;
public :
	using base::base;
	using typename base::individual_type;
	using typename base::individuals_references;

	virtual std::vector<Individual> crossOver(individuals_references const &)
	{
		std::vector<Individual> population_;
		return population_;
	}

	virtual std::vector<Individual> mutate(individuals_references const &)
	{
		std::vector<Individual> population_;
		return population_;
	}
	virtual bool goalReached()
	{
		return false;
	}
};

template<class GoalFunction>
testEvolutionaryConfig<GoalFunction> make_testEvolutionaryConfig(GoalFunction goal, size_t size)
{
	return testEvolutionaryConfig<GoalFunction>(goal, size);
}


int main()
{
	auto goalFunction = [](std::bitset<3> individual)
	{
		float s = 0;
		for(size_t it = 0; it < 3; ++it)
		{
			s += individual[it];
		}
		return s;
	};
	auto t = make_testEvolutionaryConfig(goalFunction, 10);
    t.init();
	t.breed();
    return 0;
}
