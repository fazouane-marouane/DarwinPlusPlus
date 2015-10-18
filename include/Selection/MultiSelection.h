#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "ISelection.h"

namespace Darwin
{
	template<class Population, class Indices = std::vector<size_t>>
	class MultiSelection: public Interfaces::ISelection<Population, Indices>
	{
		using base = Interfaces::ISelection<Population, Indices>;
		using Map=std::map<std::string, std::unique_ptr<base>>;
	public:
		MultiSelection(Map&& _options = Map()): options(std::move(_options)){}
		MultiSelection(MultiSelection const&) = delete;
		MultiSelection(MultiSelection&&) = default;
		MultiSelection& operator=(MultiSelection const&) = delete;
		MultiSelection& operator=(MultiSelection&&) = default;

		void AddOption(std::pair<std::string, std::unique_ptr<base>>&& option)
		{
			options.insert(std::move(option));
		}

		void AddOption(std::string name, std::unique_ptr<base>&& option)
		{
			options.insert(std::make_pair(name, std::move(option)));
		}

		void setDefault(std::string _default)
		{
			default_option = _default;
		}

		std::string getDefault()
		{
			return default_option;
		}
		
		Indices operator()(Population& population)
		{
			return (this->operator())(population, default_option);
		}

		Indices operator()(Population& population, std::string method)
		{
			auto itr = options.find(method);
			if(itr != std::end(options))
			{
				return (*(itr->second.get()))(population);
			}
			else
				throw std::runtime_error("There is no selection method called: "+method);
		}

	protected:
		Map& getOptions()
		{
			return options;
		}

		Map const& getOptions() const
		{
			return options;
		}

	private:
		Map options;
		std::string default_option;
	};

	template<class Option, class... Args>
	std::pair<std::string, std::unique_ptr<Option>> make_selection_pair(std::string name, Args&&... args)
	{
		return std::make_pair(name, std::move( make_selection<Option>(std::forward<Args>(args)...)));
	}
}
