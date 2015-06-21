#pragma once
#include<random>
#include <type_traits>

namespace Darwin
{
	namespace Rand
	{
		namespace details
		{
			class integral_type;
			class floating_type;
			class other_type;

			template<class T, bool isFloating, bool isIntegral>
			struct tag
			{
				using type = other_type;
			};

			template<class T, bool isFloating>
			struct tag<T, isFloating, true>
			{
				using type = integral_type;
			};

			template<class T>
			struct tag<T, true, false>
			{
				using type = floating_type;
			};

			template<class Type, class tag>
			class uniform_distribution
			{
				static_assert(! std::is_same<tag, other_type>::value , "Please specify this distribution for your custom type");
			};

			template<class Type>
			class uniform_distribution<Type, integral_type>: public std::uniform_int_distribution<Type>
			{
				using base = std::uniform_int_distribution<Type>;
			public:
				using base::base;
			};

			template<class Type>
			class uniform_distribution<Type, floating_type>: public std::uniform_real_distribution<Type>
			{
				using base = std::uniform_real_distribution<Type>;
			public:
				using base::base;
			};
		}

		template<class Type>
		class uniform_distribution: public details::uniform_distribution<Type, typename details::tag<Type, std::is_arithmetic<Type>::value, std::is_integral<Type>::value>::type>
		{
			using base = details::uniform_distribution<Type, typename details::tag<Type, std::is_arithmetic<Type>::value, std::is_integral<Type>::value>::type>;
		public:
			using base::base;
		};
	}
}
