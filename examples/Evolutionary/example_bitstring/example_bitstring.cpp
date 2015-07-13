// std
#include <iostream>
#include <functional>
// darwin
#include <modules/bitstring/bitstring.h>
#include <GeneticAlgorithms.h>
#include <Initialization/RandomInitialization.h>
#include <Selection/ThresholdSelection.h>
#include <Selection/UniformSelection.h>

using Individual = std::vector<bool>;
using Population = std::vector<Individual>;

template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>
{
	using base = Darwin::Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>;
public :
	testEvolutionaryConfig(GoalFunction goal, size_t dimension): base(goal), mutation_dist(0, dimension-1)
	{}

	Individual crossOver(Individual const& lhs, Individual const& rhs)
	{
		if(dist(gen))
			return lhs & rhs;
		else
			return lhs | rhs;
	}

	Individual mutate(Individual m)
	{
		auto pos = mutation_dist(gen);
		m[pos] = !m[pos];
		return m;
	}

	virtual bool goalReached()
	{
		return counter++ > 100;
	}

	void print()
	{
		auto& population = this->getPopulation();
		for (auto individual : population)
			std::cout << individual << " ";
		std::cout << std::endl;
	}

	void printBest() const
	{
		auto& goalFunction = this->getGoalFunction();
		auto& population = this->getPopulation();
		auto best = *std::max_element(std::begin(population), std::end(population), [&goalFunction](auto lhs, auto rhs)
		{ return goalFunction(lhs) < goalFunction(rhs); });
		std::cout << "count: " << counter << " -- best score: " << goalFunction(best) << std::endl;
		std::cout<< " --best :"<< best << std::endl;
	}

private:
	size_t counter = 0;
	std::bernoulli_distribution dist;
	Darwin::Rand::uniform_distribution<size_t> mutation_dist;
	std::mt19937 gen = std::mt19937(std::random_device()());
};

template<class GoalFunction>
testEvolutionaryConfig<GoalFunction> make_testEvolutionaryConfig(GoalFunction goal, size_t dimension)
{
	return testEvolutionaryConfig<GoalFunction>(goal, dimension);
}


int main()
{
	using namespace Darwin;
	auto goalFunction = [](std::vector<bool> const& individual)
	{
		float s = 0;
		for(auto v: individual)
		{
			s += v;
		}
		return s;
	};

	int population_size = 100;
	size_t dimension = 1000;
	double alpha_mutate = 0.4;
	double alpha_crossOver = 0.6;
	double alpha_removal =(alpha_mutate+alpha_crossOver)/(1+alpha_mutate+alpha_crossOver);
	auto config = make_testEvolutionaryConfig(goalFunction, dimension);

	// settings
	config.setInitializer(make_initialization<UniformInitialization<Individual>>(population_size, dimension));
	config.setSelectionForCrossOver(make_selection<UniformSelection<Individual>>(alpha_crossOver, population_size));
	config.setSelectionForMutation(make_selection<UniformSelection<Individual>>(alpha_mutate, population_size));
	config.setSelectionForRemoval(make_selection<ThresholdSelection<Individual>>(alpha_removal));

	GeneticAlgorithmLoop(config);
	config.printBest();

    return 0;
}
