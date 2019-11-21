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
}
