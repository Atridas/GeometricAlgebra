#pragma once

namespace GA
{

	template <typename... BaseVectors>
	struct Multivector;

	template<typename Signature, int... Indexes>
	struct BaseVector
	{
		static_assert(sizeof...(Indexes) == 0);

		static constexpr int Grade = 0;
		static constexpr bool IsScalar = true;
		static constexpr bool IsVector = false;
		static constexpr bool IsAntivector = false;
		static constexpr bool IsPseudoscalar = (Signature::Positive == 0 && Signature::Negative == 0);
		static constexpr bool IsGuaranteedBlade = true;
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
		constexpr bool operator<(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return false;
		}

		template<int FirstIndex, int... ParamIndexes>
		constexpr bool operator<(BaseVector<Signature, FirstIndex, ParamIndexes...>) const noexcept
		{
			return true;
		}

		template<int... ParamIndexes>
		constexpr bool operator==(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return true;
		}

		template<int FirstIndex, int... ParamIndexes>
		constexpr bool operator==(BaseVector<Signature, FirstIndex, ParamIndexes...>) const noexcept
		{
			return false;
		}

		template<int... ParamIndexes>
		constexpr static bool HasParams() noexcept
		{
			return sizeof...(ParamIndexes) == 0;
		}
	};

	template<typename Signature, int FirstIndex, int... Indexes>
	struct BaseVector<Signature, FirstIndex, Indexes...>
	{
		static constexpr int Grade = sizeof...(Indexes) + 1;
		static constexpr bool IsScalar = false;
		static constexpr bool IsVector = (Grade == 1);
		static constexpr bool IsAntivector = (Grade == Signature::Positive + Signature::Negative - 1);
		static constexpr bool IsPseudoscalar = (Grade == Signature::Positive + Signature::Negative);
		static constexpr bool IsGuaranteedBlade = IsScalar || IsVector || IsAntivector || IsPseudoscalar;
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
		constexpr bool operator<(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			static_assert(sizeof...(ParamIndexes) == 0);
			return false;
		}

		template<int ParamFirstIndex, int... ParamIndexes>
		constexpr bool operator<(BaseVector<Signature, ParamFirstIndex, ParamIndexes...>) const noexcept
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
		constexpr static bool HasParams() noexcept
		{
			if constexpr (sizeof...(ParamIndexes) == 0)
				return false;
			else
				return HasParamsPriv<ParamIndexes...>();
		}

		template<int... ParamIndexes>
		constexpr bool operator==(BaseVector<Signature, ParamIndexes...>) const noexcept
		{
			return HasParams<ParamIndexes...>();
		}

	private:
		template<int ParamFirstIndex, int... ParamIndexes>
		constexpr static bool HasParamsPriv() noexcept
		{
			return (FirstIndex == ParamFirstIndex) && (BaseVector<Signature, Indexes...>{}.HasParams<ParamIndexes...>());;
		}
	};
}
