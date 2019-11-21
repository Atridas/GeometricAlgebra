#pragma once

namespace GA
{
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
}
