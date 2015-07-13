#pragma once
#include <map>
#include <string>
#include <memory>
#include "ISelection.h"

namespace Darwin
{
	template<class Population, class Indices>
	class MultiSelection: public Interfaces::ISelection<Population, Indices>
	{
		using base = Interfaces::ISelection<Population, Indices>;
		using Map=std::map<std::string, std::unique_ptr<base>>;
	public:
		MultiSelection(Map&& _options=Map()): options(_options){}

		void AddOption(std::pair<std::string, std::unique_ptr<base>>&& option)
		{
			options.insert(option);
		}

		void AddOption(std::string name, std::unique_ptr<base>&& option)
		{
			AddOption(std::make_pair(name, option));
		}

		void setDefault(std::string _default)
		{
			default_option = _default;
		}

		std::string getDefault()
		{
			return default_option;
		}

		Indices operator()(Population& population, std::string method = default_option)
		{
			auto itr = options.find(method);
			if(itr != std::end(options))
			{
				return itr->second();
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
		return std::make_pair(name, make_selection<Option>(std::forward<Args>(args)...));
	}
}
