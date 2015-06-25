// std
#include <iostream> // std::cout
// Darwin
#include <Random/Random.h>
#include "extend_random.h"

template<class Generator>
void example_uniform_distribution_int(Generator gen)
{
	std::cout << "Testing uniform distribution on integers" << std::endl;
	Darwin::Rand::uniform_distribution<int> dis(1,6);
	auto rand = [&]() { return dis(gen); };

	for(int count=0; count<10; ++count)
		std::cout << rand() << " ";
	std::cout << std::endl;
}

template<class Generator>
void example_uniform_distribution_real(Generator gen)
{
	std::cout << "Testing uniform distribution on real numbers" << std::endl;
	Darwin::Rand::uniform_distribution<double> dis(0,1);
	auto rand = [&]() { return dis(gen); };

	for(int count=0; count<10; ++count)
		std::cout << rand() << " ";
	std::cout << std::endl;
}

template<class Generator>
void example_uniform_distribution_string(Generator gen)
{
	std::cout << "Testing custom uniform distribution on strings" << std::endl;
	Darwin::Rand::uniform_distribution<std::string, length_equals<6>> dis;
	auto rand = [&]() { return dis(gen); };
	
	for (int count = 0; count < 10; ++count)
		std::cout <<"\""<< rand() << "\"" << " ";
	std::cout<< std::endl;
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	example_uniform_distribution_int(gen);
	example_uniform_distribution_real(gen);
	example_uniform_distribution_string(gen);

}
