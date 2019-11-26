#pragma once

#include <type_traits>

#include <Multivector.hpp>

namespace GA
{
	// ----------------------------------------------------------------------------------------------------------------

	template <int Grade, typename TMultivector>
	inline constexpr auto ProjectGrade(TMultivector v) noexcept
	{
		if constexpr (TMultivector::IsZeroMultivector)
			return Multivector<>{};
		else if constexpr (TMultivector::FirstBaseVectorGrade == Grade)
		{
			return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(v.GetFirstMultivector() + ProjectGrade<Grade>(v.others));
		}
		else
		{
			return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(ProjectGrade<Grade>(v.others));
		}
	}

	// ----------------------------------------------------------------------------------------------------------------


	template<typename TMultivector>
	inline constexpr auto GradeInvolution(TMultivector m)
	{
		if constexpr (std::is_arithmetic_v<TMultivector>) // TODO this with concepts
			return m;
		else if constexpr (TMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade = TMultivector::FirstBaseVectorGrade;
			if constexpr ( Grade % 2 == 0 )
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(m.GetFirstMultivector() + GradeInvolution(m.others));
			else
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(-m.GetFirstMultivector() + GradeInvolution(m.others));
		}
	}
	
	template<typename TMultivector>
	inline constexpr auto Reverse(TMultivector m)
	{
		if constexpr (std::is_arithmetic_v<TMultivector>) // TODO this with concepts
			return m;
		else if constexpr (TMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade = TMultivector::FirstBaseVectorGrade;
			if constexpr ( Grade % 4 == 0 || Grade % 4 == 1 )
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(m.GetFirstMultivector() + Reverse(m.others));
			else
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(-m.GetFirstMultivector() + Reverse(m.others));
		}
	}
	
	template<typename TMultivector>
	inline constexpr auto CliffordConjugation(TMultivector m)
	{
		if constexpr (std::is_arithmetic_v<TMultivector>) // TODO this with concepts
			return m;
		else if constexpr (TMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int Grade = TMultivector::FirstBaseVectorGrade;
			if constexpr ( Grade % 4 == 0 || Grade % 4 == 3 )
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(m.GetFirstMultivector() + CliffordConjugation(m.others));
			else
				return ConditionalCastTo<TMultivector::IsGuaranteedVersor, TMultivector::IsGuaranteedBlade>(-m.GetFirstMultivector() + CliffordConjugation(m.others));
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
			constexpr bool BothBlades = FirstMultivector::IsGuaranteedBlade && SecondMultivector::IsGuaranteedBlade;
			auto Recursive = OuterProduct(a.GetFirstMultivector(), b.others) + OuterProduct(a.others, b);
			if constexpr (GradeExpectedResult == GradeActualResult)
				return ConditionalCastToBlade<BothBlades>(a.GetFirstMultivector() * b.GetFirstMultivector() + Recursive);
			else
				return ConditionalCastToBlade<BothBlades>(Recursive);
		}
	}


	template<typename FirstMultivector, typename SecondMultivector, typename ThirdMultivector, typename... OtherMultivectors>
	inline constexpr auto OuterProduct(FirstMultivector a, SecondMultivector b, ThirdMultivector c, OtherMultivectors... d)
	{
		return OuterProduct(a, OuterProduct(b, c, d...));
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
			constexpr bool BothBlades = FirstMultivector::IsGuaranteedBlade && SecondMultivector::IsGuaranteedBlade;
			auto Recursive = LeftContraction(a.GetFirstMultivector(), b.others) + LeftContraction(a.others, b);
			if constexpr (GradeExpectedResult == GradeActualResult)
				return ConditionalCastToBlade<BothBlades>(a.GetFirstMultivector() * b.GetFirstMultivector() + Recursive);
			else
				return ConditionalCastToBlade<BothBlades>(Recursive);
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
			constexpr bool BothBlades = FirstMultivector::IsGuaranteedBlade && SecondMultivector::IsGuaranteedBlade;
			auto Recursive = RightContraction(a.GetFirstMultivector(), b.others) + RightContraction(a.others, b);
			if constexpr (GradeExpectedResult == GradeActualResult)
				return ConditionalCastToBlade<BothBlades>(a.GetFirstMultivector() * b.GetFirstMultivector() + Recursive);
			else
				return ConditionalCastToBlade<BothBlades>(Recursive);
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
			constexpr bool BothBlades = FirstMultivector::IsGuaranteedBlade && SecondMultivector::IsGuaranteedBlade;
			auto Recursive = InnerProduct(a.GetFirstMultivector(), b.others) + InnerProduct(a.others, b);
			if constexpr (GradeExpectedResult == GradeActualResult)
				return ConditionalCastToBlade<BothBlades>(a.GetFirstMultivector() * b.GetFirstMultivector() + Recursive);
			else
				return ConditionalCastToBlade<BothBlades>(Recursive);
		}
	}

	template<typename FirstMultivector, typename SecondMultivector>
	inline constexpr auto ScalarProduct(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			constexpr bool BothBlades = FirstMultivector::IsGuaranteedBlade && SecondMultivector::IsGuaranteedBlade;
			auto Recursive = ScalarProduct(a.GetFirstMultivector(), b.others) + ScalarProduct(a.others, b);
			if constexpr (GradeActualResult == 0)
				return ConditionalCastToBlade<BothBlades>(Reverse(a.GetFirstMultivector()) * b.GetFirstMultivector() + Recursive);
			else
				return ConditionalCastToBlade<BothBlades>(Recursive);
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	template<typename... BaseVectors>
	inline constexpr auto Normalized(Versor<BaseVectors...> v)
	{
		auto NormSquared = ScalarProduct(v, v);
		return ConditionalCastToVersor<true>(v * std::sqrt(1 / NormSquared.value));
	}

	template<typename... BaseVectors>
	inline constexpr auto Normalized(Blade<BaseVectors...> v)
	{
		auto NormSquared = ScalarProduct(v, v);
		return ConditionalCastToBlade<true>(v * std::sqrt(1 / NormSquared.value));
	}

	// ----------------------------------------------------------------------------------------------------------------

	template<typename... BaseVectors>
	inline constexpr auto Inverse(Versor<BaseVectors...> v)
	{
		auto NormSquared = ScalarProduct(v, v);
		return ConditionalCastToVersor<true>(Reverse(v * (1 / NormSquared.value)));
	}

	template<typename... BaseVectors>
	inline constexpr auto Inverse(Blade<BaseVectors...> v)
	{
		auto NormSquared = ScalarProduct(v, v);
		return ConditionalCastToBlade<true>(Reverse(v * (1 / NormSquared.value)));
	}

	// ----------------------------------------------------------------------------------------------------------------


	template<typename Versor, typename Blade> // TODO guarantee through concepts
	inline constexpr auto VersorProduct(Versor a, Blade b)
	{
		static_assert(Versor::IsGuaranteedVersor && Blade::IsGuaranteedBlade);
		auto p1 = a * b;
		auto inv = Inverse(a);
		auto result = p1 * inv;
		auto projectedResult = ProjectGrade<Blade::Grade>(result);
		return ConditionalCastToBlade<true>(projectedResult);
	}
}
