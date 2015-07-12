// std
#include <iostream>
// darwin
#include <modules/permutation/permutation.h>

int main()
{
        Darwin::Permutation p(5);
        std::cout << p << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        Darwin::Rand::uniform_distribution<Darwin::Permutation> dist(5);
        auto p1 = dist(gen);
        auto p2 = dist(gen);
        std::cout<< p1 <<std::endl;
        std::cout<< p2 <<std::endl;
        std::cout<< p1*p2<<std::endl;
}
