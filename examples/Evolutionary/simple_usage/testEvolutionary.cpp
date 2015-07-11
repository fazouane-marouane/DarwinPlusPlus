// std
#include <iostream>
#include <functional>
// darwin
#include <modules/bitstring/bitstring.h>
#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include <eigen3/Eigen/Dense>

using namespace Eigen;
using Individual = std::vector<bool>;

template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>
{
	using base = Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>;
public :
	using base::base;
	testEvolutionaryConfig(GoalFunction goal, size_t Population_size, size_t dimension): base(goal, Population_size, Darwin::Rand::uniform_distribution<Individual>(dimension)), mutation_dist(0, dimension-1)
	{
	}

	using typename base::individual_type;
	using typename base::individuals_references;
	using base::population;
	using base::goalFunction;

	virtual std::vector<Individual> crossOver(individuals_references const & parents)
	{
		std::vector<Individual> population_;
		for(auto itr = std::begin(parents); itr != std::end(parents); ++itr)
		{
			auto next_itr = std::next(itr);
			if (next_itr == std::end(parents))
				next_itr = std::begin(parents);
			population_.push_back(crossOver(*itr, *next_itr));
		}
		return population_;
	}

	Individual crossOver(Individual const& lhs, Individual const& rhs)
	{
		if(dist(gen))
			return lhs & rhs;
		else
			return lhs | rhs;
	}

	virtual std::vector<Individual> mutate(individuals_references const & mutants)
	{
		std::vector<Individual> population_;
		for (auto m : mutants)
			population_.push_back(mutate(m));
		return population_;
	}

	Individual mutate(Individual m)
	{
		auto pos = mutation_dist(gen);
		m[pos] = !m[pos];
		return m;
	}

	void print()
	{
		for (auto individual : population)
			std::cout << individual << " ";
		std::cout << std::endl;
	}

	virtual bool goalReached()
	{
		return counter++ > 10;
	}

	void printBest() const
	{
		auto best = *std::max_element(std::begin(population), std::end(population), [this](auto lhs, auto rhs)
		{ return goalFunction(lhs) < goalFunction(rhs); });
		std::cout << "count: " << counter << " -- best score: " << goalFunction(best) << std::endl;
		std::cout<< " --best :"<< best << std::endl;
	}

	size_t counter = 0;
	std::bernoulli_distribution dist;
	Darwin::Rand::uniform_distribution<size_t> mutation_dist;
	std::mt19937 gen = std::mt19937(std::random_device()());
};

template<class GoalFunction>
testEvolutionaryConfig<GoalFunction> make_testEvolutionaryConfig(GoalFunction goal, size_t size, size_t dimension)
{
	return testEvolutionaryConfig<GoalFunction>(goal, size, dimension);
}


int main()
{
	int nbCities = 50;
	MatrixXf cities(2,nbCities);
	for (int i = 0; i < nbCities; i++)
	{
		cities(1,i) = i%5;
		cities(2,i) = i/5; 
	}

	// MatrixXf cityMap(nbCities, nbCities);
	// for (int i = 0; i < nbCities; i++)
	// 	for (int j = 0; j < nbCities; j++)
	// 		cityMap(i,j)= sqrt(i^2 + j^2);

	auto goalFunction = [](std::vector<int> individual)
	{
		// float s = 0;
		// for (int i = 0; i < individual.size()-1; i++)
		// {
		// 	//s += cityMap(individual[i],individual[i+1]); 
		// 	s -= sqrt((individual[i]/5-individual[i+1]/5)^2 + ((individual[i]%5)-individual[i+1]%5)^2);
		// 	for (int j = 0; j < individual.size()-1; j++)
		// 	{
		// 		s -= ((individual[i] == individual[j]) ? 100 : 0);
		// 	}
		// }
		// return s;
		float s = 0;
		for (auto v : individual){
			s += v;
		}
		return s;
	};

	// Get result of maximisation

	auto config = make_testEvolutionaryConfig(goalFunction, 100, nbCities*nbCities);
	Darwin::GeneticAlgorithmLoop(config);
	//config.printBest();

    return 0;
}
