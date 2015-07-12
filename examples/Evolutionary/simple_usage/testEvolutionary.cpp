// std
#include <iostream>
#include <functional>
// darwin
#include <modules/bitstring/bitstring.h>
#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include <Eigen/Dense>
#include <Random/discrete_distribution.h>
#include <modules/permutation/permutation.h>

using namespace Eigen;
using Individual = Darwin::Permutation;



template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>
{
	using base = Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>;
public :
	using base::base;
	using base::population_size;
    testEvolutionaryConfig(GoalFunction goal, size_t Population_size, size_t dimension): base(goal, Population_size, Darwin::Rand::uniform_distribution<Darwin::Permutation>(dimension)), mutation_dist(0, dimension-1)
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
    
    virtual void initializePopulation(std::vector<Individual> & population, std::string method ="uniform")
    {
        while( population.size() <= population_size)
            population.push_back(dist_initialization(gen));
    }

	Individual crossOver(Individual const& lhs, Individual const& rhs)
	{
		if (lhs.get().size() != rhs.get().size())
			throw std::runtime_error("Two population do not have same size");
		std::vector<size_t> v(lhs.get().size());
		for (int i = 0; i < lhs.get().size() ; i++){
			if (i%2 == 0) 
				v[i] = lhs.get()[i];
			else 
				v[i] = rhs.get()[i];
		}
		return Individual(v);
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
		Darwin::Rand::uniform_distribution<int> dist(1,m.get().size()-1);
		int permutation_index = dist(gen);
		if (permutation_index != m.get().size()-1){
			m.get()[permutation_index] = m.get()[permutation_index + 1];
			m.get()[permutation_index + 1] = m.get()[permutation_index]; 
		}
		else{
			m.get()[permutation_index] = m.get()[permutation_index-1];
			m.get()[permutation_index-1] = m.get()[permutation_index];
		}
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
		return counter++ > 10000;
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


Vector2f getCoordonnate(int i){
    Vector2f city;
    city(0) = i%5;
    city(1) = i/5;
    return city;
}



int main()
{
	int nbCities = 9;
	MatrixXf cities(2,nbCities);
	for (int i = 0; i < nbCities; i++)
	{
		cities(0,i) = i%5;
		cities(1,i) = i/5;
	}

    
	MatrixXf cityMap(nbCities, nbCities);
	for (int i = 0; i < nbCities; i++)
		for (int j = 0; j < nbCities; j++){
        	Vector2f city_i = getCoordonnate(i);
            Vector2f city_j = getCoordonnate(j);
			cityMap(i,j)= std::sqrt(std::pow(city_i(0)-city_j(0),2)+ std::pow(city_i(1)-city_j(1),2));			
		}


	auto goalFunction = [&cityMap](Individual const& individual)
	{
		float s = 0;
		for (int i = 0; i < individual.get().size()-1; i++)
		{
			s -= 10*cityMap(individual.get().at(i) % cityMap.rows(),individual.get().at(i+1) % cityMap.rows());
			//s -= sqrt((individual[i]/5-individual[i+1]/5)^2 + ((individual[i]%5)-individual[i+1]%5)^2);
			for (int j = 0; j < individual.get().size()-1; j++)
			{
				s -= ((individual.get()[i] == individual.get()[j]) ? 100 : 0);
			}
		}
		return s;
	};
	
	// // Get result of maximisation

	// auto config = make_testEvolutionaryConfig(goalFunction, 100, nbCities*nbCities);
	// Darwin::GeneticAlgorithmLoop(config);
	// //config.printBest();

	// auto goalFunction = [](std::vector<int> individual)
	// {
	// 	float s = 0;
	// 	for(auto v: individual)
	// 	{
	// 		s += v;
	// 	}
	// 	return s;
	// };
//	std::vector<double> probabilities;
//	for (int i = 0; i < nbCities; i++)
//		probabilities.push_back(1/double(nbCities));
//    std::cout << probabilities << std::endl;
	auto config = make_testEvolutionaryConfig(goalFunction, 50, 9);
	Darwin::GeneticAlgorithmLoop(config);
    config.print();
	config.printBest();
    return 0;
}
