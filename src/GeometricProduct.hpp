#pragma once

#include <BaseVector.hpp>
#include <Multivector.hpp>
#include <Addition.hpp>

namespace GA
{
	template<typename Signature, int FirstIndex, int... Indexes>
	template<int FirstParamIndex, int... ParamIndexes>
	inline constexpr auto BaseVector<Signature, FirstIndex, Indexes...>::MultType(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const noexcept
	{
		if constexpr (sizeof...(Indexes) == 0)
		{
			if constexpr (FirstIndex == FirstParamIndex)
				return BaseVector<Signature, ParamIndexes...>{};
			else if constexpr (FirstIndex < FirstParamIndex)
				return BaseVector<Signature, FirstIndex, FirstParamIndex, ParamIndexes...>{};
			else
			{
				static_assert(FirstIndex > FirstParamIndex);
				return BaseVector<Signature, FirstParamIndex>{}.MultType(BaseVector<Signature, FirstIndex>{}.MultType(BaseVector<Signature, ParamIndexes...>{}));
			}
		}
		else
		{
			return BaseVector<Signature, FirstIndex>{}.MultType(BaseVector<Signature, Indexes...>{}.MultType(v));
		}
	}

	template<typename Signature, int FirstIndex, int... Indexes>
	template<int FirstParamIndex, int... ParamIndexes>
	inline constexpr int BaseVector<Signature, FirstIndex, Indexes...>::MultSign(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const noexcept
	{
		if constexpr (sizeof...(Indexes) == 0)
		{
			if constexpr (FirstIndex == FirstParamIndex)
				return Signature::GetSignature(FirstIndex);
			else if constexpr (FirstIndex < FirstParamIndex)
				return 1;
			else
			{
				static_assert(FirstIndex > FirstParamIndex);
				constexpr int aux = BaseVector<Signature, FirstIndex>{}.MultSign(BaseVector<Signature, ParamIndexes...>{});
				return -1 * aux * BaseVector<Signature, FirstParamIndex>{}.MultSign(BaseVector<Signature, FirstIndex>{}.MultType(BaseVector<Signature, ParamIndexes...>{}));
			}
		}
		else
		{
			constexpr int aux = BaseVector<Signature, Indexes...>{}.MultSign(v);
			return aux * BaseVector<Signature, FirstIndex>{}.MultSign(BaseVector<Signature, Indexes...>{}.MultType(v));
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
	inline constexpr auto Multivector<FirstBaseVector, BaseVectors...>::operator*(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept
	{
		using FirstVectorResultType = decltype(FirstBaseVector{}.MultType(FirstOtherBaseVector{}));
		constexpr int Sign = FirstBaseVector{}.MultSign(FirstOtherBaseVector{});

		auto Result = Multivector<FirstVectorResultType>{ Sign * value * v.value } +others * Multivector<FirstOtherBaseVector>{v.value} +*this * v.others;
		using ResultType = decltype(Result);
		using OtherMultivector = Multivector<FirstOtherBaseVector, OtherBaseVectors...>;
		if constexpr (IsGuaranteedBlade && OtherMultivector::IsGuaranteedBlade)
			return ConditionalCastToBlade< ResultType::IsKVector >(ConditionalCastToVersor<true>(Result));
		else
			return Result;
	}

	template <typename... BaseVectors>
	inline constexpr auto operator*(typename Multivector<BaseVectors...>::T scalar, Multivector<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
		{
			return (typename Multivector<BaseVectors...>::T)0;
		}
		else
		{
			using ScalarMultivector = typename Multivector<BaseVectors...>::ScalarMultivector;
			return ScalarMultivector{ scalar } *v;
		}
	}

	template <typename... BaseVectors>
	inline constexpr auto operator*(Multivector<BaseVectors...> v, typename Multivector<BaseVectors...>::T scalar) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
		{
			return (typename Multivector<BaseVectors...>::T)0;
		}
		else
		{
			using ScalarMultivector = typename Multivector<BaseVectors...>::ScalarMultivector;
			return ScalarMultivector{ scalar } *v;
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <typename... BaseVectors>
	inline constexpr auto operator*(Multivector<BaseVectors...> a, typename Multivector<> b) noexcept
	{
		return b;
	}

	template <typename FirstBaseVector, typename... BaseVectors>
	inline constexpr auto operator*(Multivector<> a, typename Multivector<FirstBaseVector, BaseVectors...> b) noexcept
	{
		return a;
	}

	// ----------------------------------------------------------------------------------------------------------------


	template <typename... BaseVectors>
	template <typename... OtherBaseVectors>
	inline constexpr auto Versor<BaseVectors...>::operator*(Versor<OtherBaseVectors...> o) const noexcept
	{
		return ConditionalCastToVersor<true>((Multivector<BaseVectors...>) *this * (Multivector<OtherBaseVectors...>)o);
	}

	template <typename SingleBaseVector>
	template <typename... OtherBaseVectors>
	inline constexpr auto Versor<SingleBaseVector>::operator*(Versor<OtherBaseVectors...> o) const noexcept
	{
		return ConditionalCastToVersor<true>((Multivector<SingleBaseVector>) *this * (Multivector<OtherBaseVectors...>)o);
	}

}
