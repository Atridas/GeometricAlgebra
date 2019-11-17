#pragma once

namespace GA
{

	template<typename T, int Positive, int Negative = 0, int Zero = 0>
	struct Signature
	{
		static constexpr int Positive = Positive;
		static constexpr int Negative = Negative;
		static constexpr int Zero = Zero;

		static constexpr int NumBaseVectors = Positive + Negative + Zero;

		static constexpr int GetSignature(int index)
		{
			if (index < Positive)
				return 1;
			else if (index - Positive < Negative)
				return -1;
			else if (index - Positive - Negative < Zero)
				return 0;
			else
				return -9999999;
		}

		static constexpr int NumBase(int grade)
		{
			long long int acum = 1;
			for (long long int i = 0; i < grade; ++i)
				acum *= NumBaseVectors - i;
			for (long long int i = 0; i < grade; ++i)
				acum /= grade - i;
			return static_cast<int>(acum);
		}

		template<int N = Positive + Negative + Zero>
		static constexpr auto Pseudoscalar();
		static constexpr auto InversePseudoscalar();

		using Type = T;
	};


	template<typename Signature, int... Indexes>
	struct BaseVector
	{
		static_assert(sizeof...(Indexes) == 0);

		static constexpr int Grade = 0;

		constexpr BaseVector() {}

		template<int... ParamIndexes>
		constexpr BaseVector<Signature, ParamIndexes...> MultType(BaseVector<Signature, ParamIndexes...>) const
		{
			return BaseVector<Signature, ParamIndexes...>{};
		}

		template<int... ParamIndexes>
		constexpr int MultSign(BaseVector<Signature, ParamIndexes...>) const
		{
			return 1;
		}
	};

	template<typename Signature, int FirstIndex, int... Indexes>
	struct BaseVector<Signature, FirstIndex, Indexes...>
	{
		static constexpr int Grade = sizeof...(Indexes) + 1;

		static constexpr int First = FirstIndex;
		using ReducedBaseVector = BaseVector<Signature, Indexes...>;

		constexpr BaseVector() {}

		constexpr BaseVector<Signature, FirstIndex, Indexes...> MultType(BaseVector<Signature> v) const
		{
			return BaseVector<Signature, FirstIndex, Indexes...>{};
		}
		
		constexpr int MultSign(BaseVector<Signature> v) const
		{
			return 1;
		}

		template<int FirstParamIndex, int... ParamIndexes>
		constexpr auto MultType(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const;
		
		template<int FirstParamIndex, int... ParamIndexes>
		constexpr int MultSign(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const;
	};

	template<typename Signature, int FirstIndex, int... Indexes>
	template<int FirstParamIndex, int... ParamIndexes>
	constexpr auto BaseVector<Signature, FirstIndex, Indexes...>::MultType(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const
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
	constexpr int BaseVector<Signature, FirstIndex, Indexes...>::MultSign(BaseVector<Signature, FirstParamIndex, ParamIndexes...> v) const
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
}
