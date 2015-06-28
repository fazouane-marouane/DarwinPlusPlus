#include <ProbabilisticEvolutionaryConfig.h>
#include <GeneticAlgorithms.h>
#include <Random/Random.h>
#include <iostream>
#include <functional>

using Individual = std::bitset<3>;
class testEvolutionaryConfig : public Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>
{
	using base2 = Darwin::ProbabilisticEvolutionaryConfig<std::function<float(Individual)>, Individual>;
	public :
		using base2::base2;

	virtual std::vector<Individual> crossOver(std::list<std::reference_wrapper<Individual>> &)
	{
		std::vector<Individual> population_;
		return population_;
	}

	virtual std::vector<Individual> mutate(std::list<std::reference_wrapper<Individual>> &)
	{
		std::vector<Individual> population_;
		return population_;
	}
};

int main()
{
	auto goalFunction = [](auto individual){ int s=0; for(individual::iterator it = individual.begin(); it != individual.end(); ++it) {s = s + *it;} return s;};
	//IStandardEvolutionaryConfig<std::function<float(Individual)>, Individual, std::vector<Individual> > standardEvolutionary(goalFunction);
	testEvolutionaryConfig t(goalFunction);
    t.init();
	t.breed();
    return 0;
}
