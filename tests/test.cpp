#define BOOST_TEST_MODULE MyTests
#include <boost/test/unit_test.hpp>
#include <GeneticAlgorithms.h>
#include <Random/Random.h>
#include <iostream>

BOOST_AUTO_TEST_CASE(UniformDistribution_UsageTest_1)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	Darwin::Rand::uniform_distribution<int> dis(1,6);
	std::cout << "Testing uniform integral distribution" << std::endl;
	for(int n=0; n<10; ++n)
		std::cout << dis(gen) << ' ';
	std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(UniformDistribution_UsageTest_2)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	Darwin::Rand::uniform_distribution<double> dis(0,1);
	std::cout << "Testing uniform integral distribution" << std::endl;
	for(int n=0; n<10; ++n)
		std::cout << dis(gen) << ' ';
	std::cout << std::endl;
}
