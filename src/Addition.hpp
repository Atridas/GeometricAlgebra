#pragma once

#include <Multivector.hpp>

namespace GA
{
	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
	inline constexpr auto Multivector<FirstBaseVector, BaseVectors...>::operator+(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept
	{
		using OtherMultivector = Multivector<FirstOtherBaseVector, OtherBaseVectors...>;
		if constexpr (FirstBaseVector{} == FirstOtherBaseVector{})
		{
			if constexpr (FirstBaseVector::IsGuaranteedBlade)
				return Blade<FirstBaseVector>{ value + v.value } + (others + v.others);
			else
				return Multivector<FirstBaseVector>{ value + v.value } + (others + v.others);
		}
		else if constexpr (FirstBaseVector{} < FirstOtherBaseVector{} && sizeof...(BaseVectors) == 0)
		{
			if constexpr (IsGuaranteedBlade && OtherMultivector::IsGuaranteedBlade && FirstBaseVectorGrade == OtherMultivector::FirstBaseVectorGrade)
				return Blade<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>(Multivector<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>{value, v});
			else
				return Multivector<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>{value, v};
		}
		else  if constexpr (FirstBaseVector{} < FirstOtherBaseVector{})
			return Multivector<FirstBaseVector>{ value } + (others + v);
		else
			return Multivector<FirstOtherBaseVector>{ v.value } +(*this + v.others);
	}

	template <typename... BaseVectors>
	inline constexpr auto operator+(typename Multivector<BaseVectors...>::T scalar, Multivector<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
		{
			return scalar;
		}
		else
		{
			using ScalarMultivector = typename Multivector<BaseVectors...>::ScalarMultivector;
			return ScalarMultivector{ scalar } +v;
		}
	}

	template <typename... BaseVectors>
	inline constexpr auto operator+(Multivector<BaseVectors...> v, typename Multivector<BaseVectors...>::T scalar) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
		{
			return scalar;
		}
		else
		{
			using ScalarMultivector = typename Multivector<BaseVectors...>::ScalarMultivector;
			return ScalarMultivector{ scalar } +v;
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <typename... BaseVectors>
	inline constexpr Multivector<BaseVectors...> operator+(Multivector<BaseVectors...> a, typename Multivector<> b) noexcept
	{
		return a;
	}

	template <typename FirstBaseVector, typename... BaseVectors>
	inline constexpr Multivector<FirstBaseVector, BaseVectors...> operator+(Multivector<> a, typename Multivector<FirstBaseVector, BaseVectors...> b) noexcept
	{
		return b;
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <typename... BaseVectors>
	inline constexpr Versor<BaseVectors...> operator+(Versor<BaseVectors...> a, typename Multivector<> b) noexcept
	{
		return a;
	}

	template <typename... BaseVectors>
	inline constexpr Versor<BaseVectors...> operator+(Multivector<> a, typename Versor<BaseVectors...> b) noexcept
	{
		return b;
	}
	
	template <typename... BaseVectors>
	inline constexpr Blade<BaseVectors...> operator+(Blade<BaseVectors...> a, typename Multivector<> b) noexcept
	{
		return a;
	}

	template <typename... BaseVectors>
	inline constexpr Blade<BaseVectors...> operator+(Multivector<> a, typename Blade<BaseVectors...> b) noexcept
	{
		return b;
	}

	// ----------------------------------------------------------------------------------------------------------------


	template <typename... BaseVectors>
	inline constexpr auto operator-(Multivector<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return v;
		else
			return v.CreateFirstMultivector(-v.value) + -v.others;
	}

	template <typename... BaseVectors>
	inline constexpr auto operator-(Versor<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return v;
		else
			return Versor<BaseVectors...>(v.CreateFirstMultivector(-v.value) + -v.others);
	}

	template <typename... BaseVectors>
	inline constexpr auto operator-(Blade<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return v;
		else
			return Blade<BaseVectors...>(v.CreateFirstMultivector(-v.value) + -v.others);
	}

	// ----------------------------------------------------------------------------------------------------------------


	template <typename... BaseVectors, typename TSecondElement>
	inline constexpr auto operator-(Multivector<BaseVectors...> a, TSecondElement b) noexcept
	{
		return a + -b;
	}

	template <typename... BaseVectors>
	inline constexpr auto operator-(typename Multivector<BaseVectors...>::T a, Multivector<BaseVectors...> b) noexcept
	{
		return a + -b;
	}
}
