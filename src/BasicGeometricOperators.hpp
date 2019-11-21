#pragma once

#include <Multivector.hpp>

namespace GA
{
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
				return m.GetFirstMultivector() + GradeInvolution(m.others);
			else
				return -m.GetFirstMultivector() + GradeInvolution(m.others);
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
				return m.GetFirstMultivector() + GradeInvolution(m.others);
			else
				return -m.GetFirstMultivector() + GradeInvolution(m.others);
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
				return m.GetFirstMultivector() + GradeInvolution(m.others);
			else
				return -m.GetFirstMultivector() + GradeInvolution(m.others);
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
			if constexpr (GradeExpectedResult == GradeActualResult)
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
	inline constexpr auto ScalarProduct(FirstMultivector a, SecondMultivector b)
	{
		if constexpr (std::is_arithmetic_v< FirstMultivector> || std::is_arithmetic_v< SecondMultivector>) // TODO this with concepts
			return a * b;
		else if constexpr (FirstMultivector::IsZeroMultivector || SecondMultivector::IsZeroMultivector)
			return Multivector<>{};
		else
		{
			constexpr int GradeActualResult = GradeFromMultiplyingMultivectors<FirstMultivector, SecondMultivector>();
			if constexpr (GradeActualResult == 0)
				return CliffordConjugation(a.GetFirstMultivector()) * b.GetFirstMultivector() + ScalarProduct(a.others, b);
			else
				return ScalarProduct(a.others, b);
		}
	}
}
