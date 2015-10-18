#pragma once
#include <vector>
#include "../Random/Random.h"
#include <Random/discrete_distribution.h>
#include <algorithm>

namespace Darwin
{
    template<class Individual, class GoalFunction,class Population= std::vector<Individual>, class Indices = std::vector<std::size_t>>
    class PSelection: public Interfaces::ISelection<Population, Indices>
    {
    public:
        explicit PSelection(double _proportion, GoalFunction _goalFunction): proportion(_proportion), goalFunction(_goalFunction){}
    Indices operator()(Population& population){
        size_t N = static_cast<size_t>(population.size()*proportion); // Keep a constant size of population
        std::vector<double> probabilities(population.size());
        for (int i = 0; i < population.size(); i++){
            probabilities[i] = -goalFunction(population[i]);
        }
        /*double sum = 0;
        for (int i = 0; i < population.size(); i++){
            std::cout << probabilities[i] << std::endl;
            sum += probabilities[i];
        }
        std::cout << sum << std::endl;
        std::cout << sum - 1.0 << std::endl;*/
        Rand::discrete_distribution<double> Distribution = Rand::make_distribution_from_positive_values(probabilities);
        std::vector<size_t> v(N);
        std::generate(v.rbegin(), v.rend(), [&]() { return Distribution(gen);});
        std::sort(v.begin(), v.end(), std::greater<size_t>());
        return v;
    }
    private:
        double proportion;
        GoalFunction goalFunction;
		std::random_device rd;
		std::mt19937 gen = std::mt19937(rd());
    };
}