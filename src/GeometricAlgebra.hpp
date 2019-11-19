#pragma once

namespace GA
{

	template<typename T, int Positive, int Negative = 0>
	struct Signature
	{
		static constexpr int Positive = Positive;
		static constexpr int Negative = Negative;

		static constexpr int NumBaseVectors = Positive + Negative;

		static constexpr int GetSignature(int index) noexcept
		{
			if (index < Positive)
				return 1;
			else if (index - Positive < Negative)
				return -1;
			else
				return 0;
		}

		static constexpr int NumBase(int grade) noexcept
		{
			long long int acum = 1;
			for (long long int i = 0; i < grade; ++i)
				acum *= NumBaseVectors - i;
			for (long long int i = 0; i < grade; ++i)
				acum /= grade - i;
			return static_cast<int>(acum);
		}

		template<int N = Positive + Negative>
		static constexpr auto Pseudoscalar() noexcept;
		static constexpr auto InversePseudoscalar() noexcept;

		using Type = T;
	};

	template <typename... BaseVectors>
	struct Multivector;

	template<typename Signature, int... Indexes>
	struct BaseVector
	{
		static_assert(sizeof...(Indexes) == 0);

		static constexpr int Grade = 0;
		using BaseScalar = BaseVector<Signature>;
		using T = typename Signature::Type;

		constexpr BaseVector() noexcept {}

		template<int... ParamIndexes>
		constexpr BaseVector<Signature, ParamIndexes...> MultType(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			return BaseVector<Signature, ParamIndexes...>{};
		}

		template<int... ParamIndexes>
		constexpr int MultSign(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			return 1;
		}

		template<int... ParamIndexes>
		constexpr int operator<(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return false;
		}

		template<int FirstIndex, int... ParamIndexes>
		constexpr int operator<(BaseVector<Signature, FirstIndex, ParamIndexes...>) const noexcept
		{
			return true;
		}

		template<int... ParamIndexes>
		constexpr int operator==(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return true;
		}

		template<int FirstIndex, int... ParamIndexes>
		constexpr int operator==(BaseVector<Signature, FirstIndex, ParamIndexes...>) const noexcept
		{
			return false;
		}
	};

	template<typename Signature, int FirstIndex, int... Indexes>
	struct BaseVector<Signature, FirstIndex, Indexes...>
	{
		static constexpr int Grade = sizeof...(Indexes) + 1;
		using BaseScalar = BaseVector<Signature>;
		using T = typename Signature::Type;

		static constexpr int First = FirstIndex;
		using ReducedBaseVector = BaseVector<Signature, Indexes...>;

		constexpr BaseVector() noexcept {}

		constexpr BaseVector<Signature, FirstIndex, Indexes...> MultType(BaseVector<Signature> v) const noexcept
		{
			return BaseVector<Signature, FirstIndex, Indexes...>{};
		}
		
		constexpr int MultSign(BaseVector<Signature> v) const noexcept
		{
			return 1;
		}

		template<int FirstParamIndex, int... ParamIndexes>
		inline constexpr auto MultType(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const noexcept;
		
		template<int FirstParamIndex, int... ParamIndexes>
		inline constexpr int MultSign(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const noexcept;

		template<int... ParamIndexes>
		constexpr int operator<(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return false;
		}

		template<int ParamFirstIndex, int... ParamIndexes>
		constexpr int operator<(BaseVector<Signature, ParamFirstIndex, ParamIndexes...>) const noexcept
		{
			if constexpr (sizeof...(Indexes) < sizeof...(ParamIndexes))
				return true;
			else if constexpr (sizeof...(Indexes) > sizeof...(ParamIndexes))
				return false;
			else if constexpr (FirstIndex < ParamFirstIndex)
				return true;
			else if constexpr (FirstIndex > ParamFirstIndex)
				return false;
			else
				return BaseVector<Signature, Indexes...>{} < BaseVector<Signature, ParamIndexes...>{};
		}

		template<int... ParamIndexes>
		constexpr int operator==(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return false;
		}

		template<int ParamFirstIndex, int... ParamIndexes>
		constexpr int operator==(BaseVector<Signature, ParamFirstIndex, ParamIndexes...>) const noexcept
		{
			return (FirstIndex == ParamFirstIndex) && (BaseVector<Signature, Indexes...>{} == BaseVector<Signature, ParamIndexes...>{});
		}
	};

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


	// ---------------------------------------------------------------------------------------------



	template <typename... BaseVectors>
	struct Multivector
	{
		static_assert(sizeof...(BaseVectors) == 0);
		static constexpr int Grade = 0;
		using T = int;

		template <typename... OtherBaseVectors>
		constexpr Multivector< OtherBaseVectors...> operator+(Multivector<OtherBaseVectors...> v) const noexcept
		{
			return v;
		}

		template <typename... OtherBaseVectors>
		constexpr Multivector<> operator*(Multivector<OtherBaseVectors...>) const noexcept
		{
			return Multivector<>{};
		}
	};

	template <typename FirstBaseVector, typename... BaseVectors>
	struct Multivector<FirstBaseVector, BaseVectors...>
	{
		using TBaseVector = FirstBaseVector;
		using T = typename TBaseVector::T;
		using BaseScalar = Multivector<typename TBaseVector::BaseScalar>;
		T value;
		Multivector<BaseVectors...> others;

		template <typename... OtherBaseVectors>
		constexpr Multivector<FirstBaseVector, BaseVectors...> operator+(Multivector<OtherBaseVectors...>) const noexcept
		{
			static_assert(sizeof...(OtherBaseVectors) == 0);
			return *this;
		}

		template <typename... OtherBaseVectors>
		constexpr Multivector<> operator*(Multivector<OtherBaseVectors...>) const noexcept
		{
			static_assert(sizeof...(OtherBaseVectors) == 0);
			return Multivector<>{};
		}

		template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
		inline constexpr auto operator+(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept;

		template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
		inline constexpr auto operator*(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept;
	};

	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
	inline constexpr auto Multivector<FirstBaseVector, BaseVectors...>::operator+(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept
	{
		if constexpr (FirstBaseVector{} == FirstOtherBaseVector{})
			return Multivector<FirstBaseVector>{ value + v.value } +(others + v.others);
		else if constexpr (FirstBaseVector{} < FirstOtherBaseVector{} && sizeof...(BaseVectors) == 0)
			return Multivector<FirstBaseVector, FirstOtherBaseVector, OtherBaseVectors...>{value, v};
		else  if constexpr (FirstBaseVector{} < FirstOtherBaseVector{})
			return Multivector<FirstBaseVector>{ value } +(others + v);
		else
			return Multivector<FirstOtherBaseVector>{ v.value } + (*this + v.others);
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

	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
	inline constexpr auto Multivector<FirstBaseVector, BaseVectors...>::operator*(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept
	{
		using FirstVectorResultType = decltype(FirstBaseVector{}.MultType(FirstOtherBaseVector{}));
		constexpr int Sign = FirstBaseVector{}.MultSign(FirstOtherBaseVector{});
		return Multivector<FirstVectorResultType>{Sign * value * v.value} + others * Multivector<FirstOtherBaseVector>{v.value} + *this * v.others;
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
			using BaseScalar = typename Multivector<BaseVectors...>::BaseScalar;
			using Q = typename Multivector<BaseVectors...>::T;
			return BaseScalar{ (Q)scalar } * v;
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
			using BaseScalar = typename Multivector<BaseVectors...>::BaseScalar;
			return BaseScalar{ scalar } * v;
		}
	}
}
