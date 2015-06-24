#define BOOST_TEST_MODULE MyTests
#include <boost/test/unit_test.hpp>
#include <GeneticAlgorithms.h>
#include <Random/Random.h>
#include <iostream>

BOOST_AUTO_TEST_CASE(UniformDistribution_Test_1)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	Darwin::Rand::uniform_distribution<int> dis(1,6);
	auto rand = [&]() { return dis(gen); };

	for (int count = 0; count < 10; ++count)
		BOOST_REQUIRE( rand() <= 6);

	for (int count = 0; count < 10; ++count)
		BOOST_REQUIRE(rand() >= 1);
}
