#pragma once

#include <Multivector.hpp>

namespace GA
{
	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename... OtherBaseVectors>
	inline constexpr bool Multivector<FirstBaseVector, BaseVectors...>::operator==(Multivector<OtherBaseVectors...> m) const noexcept
	{
		if constexpr (sizeof...(OtherBaseVectors) == 0)
		{
			return value == 0 && others == m;
		}
		else
		{
			return value == m.GetFactor(FirstBaseVector{}) && others == m.RemoveBaseVector<FirstBaseVector>();
		}
	}


	template <typename... BaseVectors>
	inline constexpr bool operator==(Multivector<BaseVectors...> m, typename Multivector<BaseVectors...>::T scalar) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return scalar == 0;
		else
			return m == typename Multivector<BaseVectors...>::ScalarMultivector{ scalar };
	}

	template <typename... BaseVectors>
	inline constexpr bool operator==(typename Multivector<BaseVectors...>::T scalar, Multivector<BaseVectors...> m) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return scalar == 0;
		else
			return m == typename Multivector<BaseVectors...>::ScalarMultivector{ scalar };
	}

	template <typename... BaseVectors>
	inline constexpr bool operator==(Multivector<>, Multivector<BaseVectors...> m) noexcept
	{
		return m == 0;
	}

	// -----------------------------------------------------------------------------------------------------------------------------


	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename... OtherBaseVectors>
	inline constexpr bool Multivector<FirstBaseVector, BaseVectors...>::operator!=(Multivector<OtherBaseVectors...> m) const noexcept
	{
		return !(*this == m);
	}


	template <typename... BaseVectors>
	inline constexpr bool operator!=(Multivector<BaseVectors...> m, typename Multivector<BaseVectors...>::T scalar) noexcept
	{
		return !(m == scalar);
	}

	template <typename... BaseVectors>
	inline constexpr bool operator!=(typename Multivector<BaseVectors...>::T scalar, Multivector<BaseVectors...> m) noexcept
	{
		return !(m == scalar);
	}

	template <typename... BaseVectors>
	inline constexpr bool operator!=(Multivector<>, Multivector<BaseVectors...> m) noexcept
	{
		return !(m == 0);
	}
}
