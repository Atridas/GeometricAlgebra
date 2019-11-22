#pragma once

namespace GA
{

	template <typename... BaseVectors>
	struct Versor;
	template <typename... BaseVectors>
	struct Blade;

	template <typename... BaseVectors>
	struct Multivector
	{
		static_assert(sizeof...(BaseVectors) == 0);
		using T = int;
		using ScalarMultivector = int;
		static constexpr int FirstBaseVectorGrade = 0;
		static constexpr bool IsZeroMultivector = true;
		static constexpr bool IsKVector = true;
		static constexpr bool IsScalar = true;
		static constexpr bool IsVector = false;
		static constexpr bool IsAntivector = false;
		static constexpr bool IsPseudoscalar = false;
		static constexpr bool IsGuaranteedBlade = true;
		static constexpr bool IsGuaranteedVersor = true;

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
		constexpr bool operator==(Multivector<OtherBaseVectors...>) const noexcept;
	};

	template <typename FirstBaseVector, typename... BaseVectors>
	struct Multivector<FirstBaseVector, BaseVectors...>
	{
		using TBaseVector = FirstBaseVector;
		using T = typename TBaseVector::T;
		static constexpr int FirstBaseVectorGrade = TBaseVector::Grade;
		using ScalarMultivector = Multivector<typename TBaseVector::BaseScalar>;
		static constexpr bool IsZeroMultivector = false;
		static constexpr bool IsKVector = (sizeof...(BaseVectors) == 0 || (Multivector<BaseVectors...>::FirstBaseVectorGrade == FirstBaseVectorGrade && Multivector<BaseVectors...>::IsKVector));
		static constexpr bool IsScalar = IsKVector && TBaseVector::IsScalar;
		static constexpr bool IsVector = IsKVector && TBaseVector::IsVector;
		static constexpr bool IsAntivector = IsKVector && TBaseVector::IsAntivector;
		static constexpr bool IsPseudoscalar = IsKVector && TBaseVector::IsPseudoscalar;
		static constexpr bool IsGuaranteedBlade = IsScalar || IsVector || IsAntivector || IsPseudoscalar;
		static constexpr bool IsGuaranteedVersor = IsGuaranteedBlade;

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

		constexpr Blade<FirstBaseVector> GetFirstMultivector() const noexcept
		{
			return Blade<FirstBaseVector>{ value };
		}

		constexpr Blade<FirstBaseVector> CreateFirstMultivector(T v) const noexcept
		{
			return Blade<FirstBaseVector>{ v };
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

	template <typename... BaseVectors>
	struct Versor : Multivector<BaseVectors...>
	{
		static constexpr bool IsGuaranteedBlade = false;
		static constexpr bool IsGuaranteedVersor = true;

		explicit Versor(Multivector<BaseVectors...> m) : Multivector<BaseVectors...>(m) {}

		template <typename... OtherBaseVectors>
		inline constexpr auto operator*(Versor<OtherBaseVectors...>) const noexcept;
	};

	template <typename SingleBaseVector>
	struct Versor<SingleBaseVector> : Multivector<SingleBaseVector>
	{
		static constexpr bool IsGuaranteedBlade = false;
		static constexpr bool IsGuaranteedVersor = true;

		explicit Versor(Multivector<SingleBaseVector> m) : Multivector<SingleBaseVector>(m) {}
		explicit Versor(typename SingleBaseVector::T s) : Multivector<SingleBaseVector>{ s } {}

		template <typename... OtherBaseVectors>
		inline constexpr auto operator*(Versor<OtherBaseVectors...>) const noexcept;
	};

	template <typename... BaseVectors>
	struct Blade : Versor<BaseVectors...>
	{
		static constexpr bool IsGuaranteedBlade = true;
		static constexpr bool IsGuaranteedVersor = true;

		static constexpr int Grade = 0;
		static_assert(Grade == 0);
		explicit Blade(Multivector<BaseVectors...> m) : Versor<BaseVectors...>(m) {}
	};

	template <typename SingleBaseVector>
	struct Blade<SingleBaseVector> : Versor<SingleBaseVector>
	{
		static constexpr bool IsGuaranteedBlade = true;
		static constexpr bool IsGuaranteedVersor = true;

		static constexpr int Grade = SingleBaseVector::Grade;

		explicit Blade(Multivector<SingleBaseVector> m) : Versor<SingleBaseVector>(m) {}
		explicit Blade(typename SingleBaseVector::T s) : Versor<SingleBaseVector>(s) {}
	};

	template <typename FirstBaseVector, typename SecondBaseVector, typename... BaseVectors>
	struct Blade<FirstBaseVector, SecondBaseVector, BaseVectors...> : Versor<FirstBaseVector, SecondBaseVector, BaseVectors...>
	{
		static constexpr bool IsGuaranteedBlade = true;
		static constexpr bool IsGuaranteedVersor = true;

		static constexpr int Grade = FirstBaseVector::Grade;

		static_assert(sizeof...(BaseVectors) == 0 || Blade<SecondBaseVector, BaseVectors...>::Grade == Grade);

		explicit Blade(Multivector<FirstBaseVector, SecondBaseVector, BaseVectors...> m) : Versor<FirstBaseVector, SecondBaseVector, BaseVectors...>(m) {}
	};



	template <bool KnownVersor, bool KnownBlade, typename... BaseVectors>
	inline constexpr auto ConditionalCastTo(Multivector<BaseVectors...> m)
	{
		if constexpr (KnownBlade || Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(m);
		else if constexpr (KnownVersor)
			return Versor<BaseVectors...>(m);
		else
			return m;
	}

	template <bool KnownVersor, bool KnownBlade, typename... BaseVectors>
	inline constexpr auto ConditionalCastTo(Versor<BaseVectors...> v)
	{
		if constexpr (KnownBlade || Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(v);
		else
			return v;
	}

	template <bool KnownVersor, bool KnownBlade, typename... BaseVectors>
	inline constexpr auto ConditionalCastTo(Blade<BaseVectors...> b)
	{
		return b;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToVersor(Multivector<BaseVectors...> m)
	{
		if constexpr (Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(m);
		else if constexpr (Cast)
			return Versor<BaseVectors...>(m);
		else
			return m;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToVersor(Versor<BaseVectors...> v)
	{
		if constexpr (Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(v);
		else
			return v;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToVersor(Blade<BaseVectors...> b)
	{
		return b;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToBlade(Multivector<BaseVectors...> m)
	{
		if constexpr (Cast || Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(m);
		else
			return m;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToBlade(Versor<BaseVectors...> v)
	{
		if constexpr (Cast || Multivector < BaseVectors...>::IsGuaranteedBlade)
			return Blade<BaseVectors...>(v);
		else
			return v;
	}

	template <bool Cast, typename... BaseVectors>
	inline constexpr auto ConditionalCastToBlade(Blade<BaseVectors...> b)
	{
		return b;
	}

}
