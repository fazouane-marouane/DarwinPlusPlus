// std
#include <iostream> // std::cout
#include <map>
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
#include <cmath>

template<class Generator>
void example_uniform_distribution_string2(Generator gen)
{
	std::cout << "Testing custom uniform distribution on strings" << std::endl;
	Darwin::Rand::uniform_distribution<std::string, length_less_than<3>> dis('a','b');
	auto rand = [&]() { return dis(gen); };
	std::map<std::string, size_t, Darwin::utility::string_length_then_alphabetical_ordering> histogram;
	size_t number_of_samples = 1'000'000;
	double mean = number_of_samples / 14;

	for (size_t count = 0; count < number_of_samples; ++count)
	{
		auto word = rand();
		histogram[word]++;
	}
	std::cout << "histogram size: " << histogram.size() << std::endl;
	double deviation = 0.0;
	for (auto& entry : histogram)
	{
		deviation += std::pow(entry.second - mean,2);
		std::cout <<" --> "<< entry.first << ":\t" << entry.second << std::endl;
	}
	std::cout << std::endl;
	deviation = std::sqrt(deviation);
	std::cout <<"deviation: "<< deviation << std::endl;
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	example_uniform_distribution_int(gen);
	example_uniform_distribution_real(gen);
	example_uniform_distribution_string(gen);
	example_uniform_distribution_string2(gen);
}
