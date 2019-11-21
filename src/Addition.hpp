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
			if constexpr (FirstBaseVector::IsScalar)
				return Scalar<FirstBaseVector>{ value + v.value } + (others + v.others);
			else if constexpr (FirstBaseVector::IsVector)
				return Vector<FirstBaseVector>{ value + v.value } + (others + v.others);
			else if constexpr (FirstBaseVector::IsGuaranteedBlade)
				return Blade<FirstBaseVector>{ value + v.value } + (others + v.others);
			else
				return Multivector<FirstBaseVector>{ value + v.value } + (others + v.others);
		}
		else if constexpr (FirstBaseVector{} < FirstOtherBaseVector{} && sizeof...(BaseVectors) == 0)
		{
			if constexpr (IsVector && OtherMultivector::IsVector)
				return Vector<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>(Multivector<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>{value, v});
			else if constexpr (IsGuaranteedBlade && OtherMultivector::IsGuaranteedBlade && FirstBaseVectorGrade == OtherMultivector::FirstBaseVectorGrade)
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
			using BaseScalar = typename Multivector<BaseVectors...>::BaseScalar;
			using Q = typename Multivector<BaseVectors...>::T;
			return BaseScalar{ (Q)scalar } +v;
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
			using BaseScalar = typename Multivector<BaseVectors...>::BaseScalar;
			return BaseScalar{ scalar } +v;
		}
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
}
