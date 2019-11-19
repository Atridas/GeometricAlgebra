#pragma once

#include <type_traits>

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
		using BaseScalar = int;
		static constexpr bool IsZeroMultivector = true;

		template<int... Indexes>
		constexpr T GetFactor() const noexcept
		{
			return 0;
		}

		template<typename BaseVector>
		constexpr Multivector<> RemoveBaseVector() const noexcept
		{
			return Multivector<>{};
		}

		template <typename... OtherBaseVectors>
		constexpr Multivector< OtherBaseVectors...> operator+(Multivector<OtherBaseVectors...> v) const noexcept
		{
			return v;
		}

		template <typename... OtherBaseVectors>
		constexpr Multivector< OtherBaseVectors...> operator-(Multivector<OtherBaseVectors...> v) const noexcept
		{
			return -v;
		}

		template <typename... OtherBaseVectors>
		constexpr Multivector<> operator*(Multivector<OtherBaseVectors...>) const noexcept
		{
			return Multivector<>{};
		}

		template <typename... OtherBaseVectors>
		constexpr bool operator==(Multivector<OtherBaseVectors...>) const noexcept;
	};

	template <typename FirstBaseVector, typename... BaseVectors>
	struct Multivector<FirstBaseVector, BaseVectors...>
	{
		using TBaseVector = FirstBaseVector;
		using T = typename TBaseVector::T;
		static constexpr int FirstBaseVectorGrade = TBaseVector::Grade;
		using BaseScalar = Multivector<typename TBaseVector::BaseScalar>;
		static constexpr bool IsZeroMultivector = false;
		T value;
		Multivector<BaseVectors...> others;

		template<int... Indexes>
		constexpr T GetFactor() const noexcept
		{
			if constexpr (TBaseVector::template HasParams<Indexes...>())
			{
				return value;
			}
			else
			{
				return others.GetFactor<Indexes...>();
			}
		}

		template<typename Signature, int... Indexes>
		constexpr T GetFactor(BaseVector<Signature, Indexes...>) const noexcept
		{
			return GetFactor<Indexes...>();
		}

		template<typename BaseVectorFactor>
		constexpr T GetFactor(Multivector<BaseVectorFactor>) const noexcept
		{
			return GetFactor(BaseVectorFactor{});
		}

		template<typename BaseVector>
		constexpr auto RemoveBaseVector() const noexcept
		{
			if constexpr (FirstBaseVector{} == BaseVector{})
				return others;
			else
				return GetFirstMultivector() + others.RemoveBaseVector<BaseVector>();
		}

		constexpr Multivector<FirstBaseVector> GetFirstMultivector() const noexcept
		{
			return Multivector<FirstBaseVector>{ value };
		}

		constexpr Multivector<FirstBaseVector> CreateFirstMultivector(T v) const noexcept
		{
			return Multivector<FirstBaseVector>{ v };
		}

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

		template <typename... OtherBaseVectors>
		constexpr auto operator-(Multivector<OtherBaseVectors...> v) const noexcept
		{
			return *this + -v;
		}

		template <typename FirstOtherBaseVector, typename... OtherBaseVectors>
		inline constexpr auto operator*(Multivector<FirstOtherBaseVector, OtherBaseVectors...> v) const noexcept;

		template <typename... OtherBaseVectors>
		constexpr bool operator==(Multivector<OtherBaseVectors...>) const noexcept;
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

	// ----------------------------------------------------------------------------------------------------------------


	template <typename... BaseVectors>
	inline constexpr auto operator-(Multivector<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return v;
		else
			return v.CreateFirstMultivector(-v.value) + -v.others;
	}

	// ----------------------------------------------------------------------------------------------------------------

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

	// ----------------------------------------------------------------------------------------------------------------

	template <typename FirstBaseVector, typename... BaseVectors>
	template <typename... OtherBaseVectors>
	inline constexpr bool Multivector<FirstBaseVector, BaseVectors...>::operator==(Multivector<OtherBaseVectors...> m) const noexcept
	{
		if constexpr (sizeof...(OtherBaseVectors) == 0)
		{
			return GetFactor<>() == 0 && others == m;
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
			return m == typename Multivector<BaseVectors...>::BaseScalar{ scalar };
	}

	template <typename... BaseVectors>
	inline constexpr bool operator==(typename Multivector<BaseVectors...>::T scalar, Multivector<BaseVectors...> m) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return scalar == 0;
		else
			return m == typename Multivector<BaseVectors...>::BaseScalar{ scalar };
	}

	template <typename... BaseVectors>
	template <typename... OtherBaseVectors>
	inline constexpr bool Multivector<BaseVectors...>::operator==(Multivector<OtherBaseVectors...> m) const noexcept
	{
		static_assert(sizeof...(BaseVectors) == 0);
		return m == 0;
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <int Grade, typename... BaseVectors>
	inline constexpr auto ProjectGrade(Multivector<BaseVectors...> v) noexcept
	{
		if constexpr (sizeof...(BaseVectors) == 0)
			return Multivector<>{};
		else if constexpr (Multivector<BaseVectors...>::FirstBaseVectorGrade == Grade)
		{
			return v.GetFirstMultivector() + ProjectGrade<Grade>(v.others);
		}
		else
		{
			return ProjectGrade<Grade>(v.others);
		}
	}

	// ----------------------------------------------------------------------------------------------------------------


	template<typename FirstBaseVector, typename SecondBaseVector>
	inline constexpr int GradeFromMultiplyingBaseVectors() noexcept
	{
		constexpr auto r = FirstBaseVector{}.MultType(SecondBaseVector{});
		return r.Grade;
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr int GradeFromMultiplyingMultivectors() noexcept
	{
		return GradeFromMultiplyingBaseVectors<typename FirstMultivector::TBaseVector, typename SecondMultivector::TBaseVector>();
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto OuterProduct(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade1 = FirstMultivector::FirstBaseVectorGrade;
			constexpr int Grade2 = SecondMultivector::FirstBaseVectorGrade;
			constexpr int GradeExpectedResult = Grade1 + Grade2;
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			if constexpr(GradeExpectedResult == GradeActualResult)
				return a.GetFirstMultivector() * b.GetFirstMultivector() + OuterProduct(a.others, b);
			else
				return OuterProduct(a.others, b);
		}
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto LeftContraction(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade1 = FirstMultivector::FirstBaseVectorGrade;
			constexpr int Grade2 = SecondMultivector::FirstBaseVectorGrade;
			constexpr int GradeExpectedResult = Grade2 - Grade1;
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			if constexpr (GradeExpectedResult == GradeActualResult)
				return a.GetFirstMultivector() * b.GetFirstMultivector() + LeftContraction(a.others, b);
			else
				return LeftContraction(a.others, b);
		}
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto RightContraction(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade1 = FirstMultivector::FirstBaseVectorGrade;
			constexpr int Grade2 = SecondMultivector::FirstBaseVectorGrade;
			constexpr int GradeExpectedResult = Grade1 - Grade2;
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			if constexpr (GradeExpectedResult == GradeActualResult)
				return a.GetFirstMultivector() * b.GetFirstMultivector() + RightContraction(a.others, b);
			else
				return RightContraction(a.others, b);
		}
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto InnerProduct(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade1 = FirstMultivector::FirstBaseVectorGrade;
			constexpr int Grade2 = SecondMultivector::FirstBaseVectorGrade;
			constexpr int GradeExpectedResult = (Grade1 > Grade2) ? Grade1 - Grade2 : Grade2 - Grade1;
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			if constexpr (GradeExpectedResult == GradeActualResult)
				return a.GetFirstMultivector() * b.GetFirstMultivector() + InnerProduct(a.others, b);
			else
				return InnerProduct(a.others, b);
		}
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto StarProduct(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade1 = FirstMultivector::FirstBaseVectorGrade;
			constexpr int Grade2 = SecondMultivector::FirstBaseVectorGrade;
			if constexpr(Grade1 == Grade2)
				return a.GetFirstMultivector() * b.GetFirstMultivector() + StarProduct(a.others, b);
			else
				return StarProduct(a.others, b);
		}
	}
}
