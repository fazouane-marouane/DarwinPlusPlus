#pragma once
#include <algorithm>
#include <iterator>
#include "ISelection.h"
#include "../Random/Random.h"

namespace Darwin
{
    class NotImplementedException : public std::logic_error
    {
    public:
        NotImplementedException(std::string name) : std::logic_error(name+" not yet implemented.") {}
    };

    template<class Population, class Indices>
    class NotImplementedSelection: public Interfaces::ISelection<Population, Indices>
    {
    public:
        explicit NotImplementedSelection(){}

        Indices operator()(Population& /*population*/)
        {
            throw NotImplementedException("Selection method");
        }
    };
}
