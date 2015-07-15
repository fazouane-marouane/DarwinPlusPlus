#pragma once
#include <boost/optional.hpp>

namespace Darwin
{
	template<class WrappedType, class MemoizedValueType>
	class Memoized
	{
	public:
		template<class WrappedType2 = WrappedType>
		Memoized(WrappedType2 const& m) : wrapped_value(m)
		{}

		template<class WrappedType2 = WrappedType, class MemoizedValueType2 = MemoizedValueType>
		Memoized(Memoized<WrappedType2, MemoizedValueType2>&& m) : wrapped_value(m.wrapped_value), value(m.wrapped_value)
		{}

		WrappedType const& get() const
		{
			return wrapped_value;
		}

		WrappedType& get()
		{
			return wrapped_value;
		}

		void set(WrappedType const& other)
		{
			wrapped_value = other;
			value = boost::none;
		}

		void resetMemoizedValue()
		{
			value = boost::none;
		}

		void setMemoizedValue(MemoizedValueType const& other)
		{
			value = other;
		}

		MemoizedValueType const& getMemoizedValue() const
		{
			return value.get();
		}

		bool hasMemoizedValue() const
		{
			return value.is_initialized();
		}
	private:
		WrappedType wrapped_value;
		boost::optional<MemoizedValueType> value = boost::none;
	};

	template<class Function>
	inline auto liftMemoized(Function f)
	{
		return [f](auto& m)
		{
			if(! m.hasMemoizedValue())
				m.setMemoizedValue(f(m.get()));
			return m.getMemoizedValue();
		};
	}
}
