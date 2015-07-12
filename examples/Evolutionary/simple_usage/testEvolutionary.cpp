// std
#include <iostream>
#include <functional>
// darwin
#include <modules/bitstring/bitstring.h>
#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include <eigen3/Eigen/Dense>
#include <Random/discrete_distribution.h>

using namespace Eigen;
using Individual = std::vector<int>;

namespace Darwin
{
	namespace Rand
	{
		template<>
		class uniform_distribution<std::vector<int>>
		{
		public:
			uniform_distribution(size_t _size, std::vector<double> probabilities): size(_size), dists(make_distribution(probabilities))
			{}
			template<class Generator>
			std::vector<int> operator()(Generator& gen)
			{
				std::vector<int> ret;
				ret.reserve(size);
				for(size_t position = 0; position < size; ++position)
				{
					ret.push_back(dists(gen));
				}
				return ret;
			}
		private:
			discrete_distribution<double> dists;
			size_t const size;
		};
	}
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec)
{
    os << '[ ';
    for (auto& el : vec)
    {
        os << el << ' ';
    }
    os << ' ]';
    return os;
}


template<class GoalFunction>
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>
{
	using base = Darwin::ProbabilisticEvolutionaryConfig<GoalFunction, Individual, std::vector<Individual>>;
public :
	using base::base;
	testEvolutionaryConfig(GoalFunction goal, size_t Population_size, size_t dimension, std::vector<double> probabilities): base(goal, Population_size, Darwin::Rand::uniform_distribution<Individual>(dimension, probabilities)), mutation_dist(0, dimension-1)
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
		if (lhs.size() != rhs.size())
			throw std::runtime_error("Two population do not have same size");
		std::vector<int> v(lhs.size());
		for (int i = 0; i < lhs.size() ; i++){
			if (i%2 == 0) 
				v[i] = lhs[i];
			else 
				v[i] = rhs[i];
		}
		return v;
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
		Darwin::Rand::uniform_distribution<int> dist(1,m.size()-1);
		int permutation_index = dist(gen);
		if (permutation_index != m.size()-1){
			m[permutation_index] = m[permutation_index + 1];
			m[permutation_index + 1] = m[permutation_index]; 
		}
		else{
			m[permutation_index] = m[permutation_index-1];
			m[permutation_index-1] = m[permutation_index];
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
		return counter++ > 50;
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
testEvolutionaryConfig<GoalFunction> make_testEvolutionaryConfig(GoalFunction goal, size_t size, size_t dimension, std::vector<double> probabilities)
{
	return testEvolutionaryConfig<GoalFunction>(goal, size, dimension, probabilities);
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
		for (int j = 0; j < nbCities; j++)
			cityMap(i,j)= sqrt(i^2 + j^2);

	auto goalFunction = [&cityMap](std::vector<int> individual)
	{
		float s = 0;
		for (int i = 0; i < individual.size()-1; i++)
		{
			s -= cityMap(individual.at(i) % cityMap.rows(),individual.at(i+1) % cityMap.rows());
			//s -= sqrt((individual[i]/5-individual[i+1]/5)^2 + ((individual[i]%5)-individual[i+1]%5)^2);
			for (int j = 0; j < individual.size()-1; j++)
			{
				s -= ((individual[i] == individual[j]) ? 100 : 0);
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
	std::vector<double> probabilities;
	for (int i = 0; i < nbCities; i++)
		probabilities.push_back(1/double(nbCities));
	auto config = make_testEvolutionaryConfig(goalFunction, 20, nbCities, probabilities);
	Darwin::GeneticAlgorithmLoop(config);
    config.print();
	config.printBest();
    return 0;
}
