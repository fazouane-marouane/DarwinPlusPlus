// std
#include <iostream>
#include <functional>
// darwin
#include <IEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include <Eigen/Dense>
#include <modules/permutation/permutation.h>
#include <Initialization/RandomInitialization.h>
#include <Selection/ThresholdSelection.h>
#include <Selection/UniformSelection.h>

using namespace Eigen;
using Individual = Darwin::Permutation;
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
		return lhs*rhs;
	}

	Individual mutate(Individual m)
	{
		int permutation_index1 = mutation_dist(gen);
		int permutation_index2 = mutation_dist(gen);
		m.transpose(permutation_index1, permutation_index2);
		return m;
	}

	virtual bool goalReached()
	{
		return counter++ > 50 ;
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
	Darwin::Rand::uniform_distribution<size_t> mutation_dist;
	std::mt19937 gen = std::mt19937(std::random_device()());
};

template<class GoalFunction>
testEvolutionaryConfig<GoalFunction> make_testEvolutionaryConfig(GoalFunction&& goal, size_t dimension)
{
	return testEvolutionaryConfig<GoalFunction>(std::forward<GoalFunction>(goal), dimension);
}


Vector2f getCoordonnate(int i){
    Vector2f city;
    city(0) = i%3;
    city(1) = i/3;
    return city;
}

int main()
{
	using namespace Darwin;
	// Data
	int nbCities = 9;
    MatrixXf cityMap(nbCities, nbCities);
	for (int i = 0; i < nbCities; i++)
		for (int j = 0; j < nbCities; j++){
        	Vector2f city_i = getCoordonnate(i);
            Vector2f city_j = getCoordonnate(j);
			cityMap(i,j)= std::sqrt(std::pow(city_i(0)-city_j(0),2)+ std::pow(city_i(1)-city_j(1),2));			
		}

	std::cout << cityMap << std::endl;

	auto goalFunction = [&cityMap](Individual const& individual)
	{
		float s = 0;
		for (int i = 0; i < individual.get().size()-1; i++)
		{
			s -= cityMap(individual.get().at(i), individual.get().at(i+1));
		}
		return s;
	};
	// Problem solving

	size_t population_size = 100;
	size_t dimension = nbCities;
	double alpha_mutate = 0.3;
	double alpha_crossOver = 0.3;
	double alpha_removal =(alpha_mutate+alpha_crossOver)/(1+alpha_mutate+alpha_crossOver);
	auto config = make_testEvolutionaryConfig(goalFunction, dimension);

	// settings
	config.setInitializer(make_initialization<UniformInitialization<Individual>>(population_size, dimension));
	config.setSelectionForCrossOver(make_selection<UniformSelection<Individual>>(alpha_crossOver, population_size));
	config.setSelectionForMutation(make_selection<UniformSelection<Individual>>(alpha_mutate, population_size));
	config.setSelectionForRemoval(make_selection<ThresholdSelection<Individual>>(alpha_removal));

	// run
	Darwin::GeneticAlgorithmLoop(config);
    config.print();
	config.printBest();
    return 0;
}
