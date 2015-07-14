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
#include <modules/Memoization.h>
#include <chrono>
#include <omp.h>

namespace Darwin
{
	namespace Rand
	{
		template<class Type, class MemoizedType>
		class uniform_distribution<Memoized<Type, MemoizedType>> : public uniform_distribution<Type>
		{
			using base = uniform_distribution<Type>;
		public:
			using base::base;

			template<class Generator>
			Memoized<Type, MemoizedType> operator()(Generator gen)
			{
				return Memoized<Type, MemoizedType>(base::operator()(gen));
			}
		};
	}
}

using namespace Eigen;
using Individual = Darwin::Memoized<Darwin::Permutation, double>;
using Population = std::vector<Individual>;

Vector2f getCoordonnate(size_t i, size_t nbCities) {
	Vector2f city;
	city(0) = i % size_t(std::sqrt(nbCities));
	city(1) = i / size_t(std::sqrt(nbCities));
	return city;
}

template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>
{
	using base = Darwin::Interfaces::IStandardEvolutionaryConfig<GoalFunction, Individual, Population>;
public :
	testEvolutionaryConfig(GoalFunction goal, size_t dimension): base(goal), mutation_dist(0, dimension-1)
	{}

	Individual crossOver(Individual const& lhs, Individual const& rhs)
	{
		Individual tmp = lhs;
		Individual tmp2 = rhs;
		tmp.get().invert();
		tmp2.get().reverse();
		return Individual(tmp.get()*tmp2.get());
	}

	Individual mutate(Individual m)
	{
		int permutation_index1 = mutation_dist(gen);
		int permutation_index2 = mutation_dist(gen);
		m.get().reverseAllBetween(permutation_index1, permutation_index2);
		return Individual(m.get());
	}

	virtual bool goalReached()
	{
		return counter++ > 50;
	}
    
	void print()
	{
		auto& population = this->getPopulation();
		for (auto individual : population)
			std::cout << individual.get() << " ";
		std::cout << std::endl;
	}

	void printBest(size_t nbcities) const
	{
		auto& goalFunction = this->getGoalFunction();
		auto& population = this->getPopulation();
		auto best = *std::max_element(std::begin(population), std::end(population), [&goalFunction](auto lhs, auto rhs)
		{ return goalFunction(lhs) < goalFunction(rhs); });
		std::cout << "count: " << counter << " -- best score: " << goalFunction(best) << std::endl;
		std::cout << " --best :"<<std::endl;// << best << std::endl;
		
		std::cout << "{";
		for(auto c: best.get().get())
		{
			auto coordinates= getCoordonnate(c, nbcities);
			std::cout << "{" << coordinates(0) << ", " << coordinates(1) << "}, ";
		}
		auto coordinates = getCoordonnate(best.get().get()[0], nbcities);
		std::cout << "{" << coordinates(0) << ", " << coordinates(1) << "}";
		std::cout << "}" << std::endl;
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

double distance2D(Vector2f lhs, Vector2f rhs)
{
	return std::sqrt(std::pow(lhs(0) - rhs(0) +0.0L, 2) + std::pow(lhs(1) - rhs(1)+0.0L, 2));
}

double distance2D(size_t lhs, size_t rhs, size_t nbCities)
{
	return distance2D(getCoordonnate(lhs,nbCities), getCoordonnate(rhs, nbCities));
}

int main()
{
	using namespace Darwin;
	// Data
	int nbCities =  100*100;
    MatrixXd cityMap(nbCities, nbCities);
	for (int i = 0; i < nbCities; i++)
		for (int j = 0; j < nbCities; j++){
        	Vector2f city_i = getCoordonnate(i,nbCities);
            Vector2f city_j = getCoordonnate(j,nbCities);
			cityMap(i, j) = distance2D(city_i, city_j);
		}

	//std::cout << cityMap << std::endl;

	auto goalFunction = [&cityMap, &nbCities](Darwin::Permutation const& individual)
	{
		double s = 0;
		#pragma omp parallel for
		for (int i = 0; i < individual.get().size()-1; i++)
		{
			s -= cityMap(individual.get().at(i), individual.get().at(i+1));
		}
		return s;
	};
	// Problem solving

	size_t population_size = 100;
	size_t dimension = nbCities;
	double alpha_mutate = 0.999;
	double alpha_crossOver = 0.999;
	double alpha_removal =(alpha_mutate+alpha_crossOver)/(1+alpha_mutate+alpha_crossOver);
	auto config = make_testEvolutionaryConfig(liftMemoized(goalFunction), dimension);

	// settings
	config.setInitializer(make_initialization<UniformInitialization<Individual>>(population_size, dimension));
	config.setSelectionForCrossOver(make_selection<UniformSelection<Individual>>(alpha_crossOver, population_size));
	config.setSelectionForMutation(make_selection<UniformSelection<Individual>>(alpha_mutate, population_size));
	config.setSelectionForRemoval(make_selection<ThresholdSelection<Individual>>(alpha_removal));

	// run
	auto start = std::chrono::steady_clock::now();
	Darwin::GeneticAlgorithmLoop(config);
	auto end = std::chrono::steady_clock::now();
    //config.print();
	config.printBest(nbCities);
	auto diff = end - start;
	std::cout << "total time: " << std::chrono::duration<double, std::micro>(diff).count() <<"µs"<<std::endl;
    return 0;
}
