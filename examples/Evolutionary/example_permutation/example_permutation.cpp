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
#include <Selection/PSelection.h>
#include <modules/Memoization.h>
#include <chrono>
#if defined(DARWIN_OPENMP)
#include <omp.h>
#endif
#include <sstream>

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
		return counter++ > 100;
	}
    
	void print(std::ostream& os)
	{
		auto& population = this->getPopulation();
		for (auto individual : population)
			os << individual.get() << " ";
		os << std::endl;
	}

	void printBest(size_t nbcities, std::ostream& os) const
	{
		auto& goalFunction = this->getGoalFunction();
		auto& population = this->getPopulation();
		auto best = *std::max_element(std::begin(population), std::end(population), [&goalFunction](auto lhs, auto rhs)
		{ return goalFunction(lhs) < goalFunction(rhs); });
		os <<"cities: "<<nbcities<< " count: " << counter << " -- best score: " << goalFunction(best) << std::endl;
		/*os << " --best :"<<std::endl;// << best << std::endl;
		
		os << "{";
		for(auto c: best.get().get())
		{
			auto coordinates= getCoordonnate(c, nbcities);
			os << "{" << coordinates(0) << ", " << coordinates(1) << "}, ";
		}
		auto coordinates = getCoordonnate(best.get().get()[0], nbcities);
		os << "{" << coordinates(0) << ", " << coordinates(1) << "}";
		os << "}" << std::endl;*/
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

#include <chrono>

void testCase(int nbCities, size_t population_size, double alpha_mutate, double alpha_crossOver)
{
	std::ostringstream strBuffer;
	using namespace Darwin;
	// Data
    MatrixXd cityMap(nbCities, nbCities);
	for (int i = 0; i < nbCities; i++)
		for (int j = 0; j < nbCities; j++){
        	Vector2f city_i = getCoordonnate(i,nbCities);
            Vector2f city_j = getCoordonnate(j,nbCities);
			cityMap(i, j) = distance2D(city_i, city_j);
		}

	std::cout << "==OK==" << std::endl;
	//strBuffer << cityMap << std::endl;

	auto goalFunction = liftMemoized([&cityMap, &nbCities](Darwin::Permutation const& individual)
	{
		double s = 0;
		#pragma omp parallel for reduction(-:s)
		for (int i = 0; i < individual.get().size()-1; i++)
		{
			s -= cityMap(individual.get().at(i), individual.get().at(i+1));
		}
		return s;
	});
	// Problem solving
	size_t dimension = nbCities;

	double alpha_removal =(alpha_mutate+alpha_crossOver)/(1+alpha_mutate+alpha_crossOver);
	auto config = make_testEvolutionaryConfig(goalFunction, dimension);

	// settings
	config.setInitializer(make_initialization<UniformInitialization<Individual>>(population_size, dimension));
	config.setSelectionForCrossOver(make_selection<UniformSelection<Individual>>(alpha_crossOver));
	config.setSelectionForMutation(make_selection<UniformSelection<Individual>>(alpha_mutate));
	config.setSelectionForRemoval(make_selection<ThresholdSelection<Individual>>(alpha_removal));


	// run
	auto start = std::chrono::steady_clock::now();
	Darwin::GeneticAlgorithmLoop(config);
	auto end = std::chrono::steady_clock::now();
	strBuffer << std::chrono::duration <double, std::micro>(end-start).count() << " microsec" << std::endl;
	auto perfs = config.getPerfs();
	for (auto const& p : perfs)
		strBuffer << p.first << ": " << std::chrono::duration <double, std::micro>(p.second).count() << " microsec" << std::endl;
    //config.print();
	config.printBest(nbCities, strBuffer);
	std::cout << strBuffer.str()<<std::endl;
}

#include <future>

int main()
{
	std::future<void> runners[] = {
		std::async(std::launch::deferred, testCase, 3*3, 100, 0.5, 0.3),
		std::async(std::launch::deferred, testCase, 10 * 10, 1000, 0.8, 0.8),
		std::async(std::launch::deferred, testCase, 20 * 20, 1000, 0.8, 0.8),
		std::async(std::launch::deferred, testCase, 30 * 30, 1000, 0.8, 0.8),
		std::async(std::launch::deferred, testCase, 50 * 50, 1000, 0.9, 0.9),
		std::async(std::launch::deferred, testCase, 50 * 50, 10000, 0.9, 0.9),
		std::async(std::launch::deferred, testCase, 100 * 100, 1000, 1.0, 1.0) };

	for (auto& r : runners)
		r.get();
}
