// std
#include <iostream> // std::cout
// Darwin
#include <Random/Random.h>
#include "extend_random.h"

template<class Generator>
void example_uniform_distribution_bitstring(Generator gen)
{
	std::cout << "Testing custom uniform distribution on bitstrings" << std::endl;
	Darwin::Rand::uniform_distribution<std::bitset<3>> dis;
	auto rand = [&]() { return dis(gen); };

	for (int count = 0; count < 10; ++count)
		std::cout <<"["<< rand() << "] ";
	std::cout<< std::endl;
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	example_uniform_distribution_bitstring(gen);

}
