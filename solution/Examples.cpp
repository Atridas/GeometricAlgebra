// GeometricAlgebra.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <type_traits>
#include <tuple>

#include <GeometricAlgebra.hpp>

#include "catch.hpp"

template<typename T, typename Q>
bool IsType(Q q)
{
	return std::is_base_of_v<T, Q>;
}


namespace GA
{

	template<typename Signature, int FirstNum, int... Nums>
	std::string GetVectorSuffix()
	{
		using namespace std::string_literals;

		std::string others;
		if constexpr (sizeof...(Nums) != 0)
			others = GetVectorSuffix< Signature, Nums...>();
		else
			others = ""s;

		if constexpr (Signature::Positive == FirstNum)
			return "n"s + others;
		else if constexpr (Signature::Positive == FirstNum + 1 && Signature::Negative != 0)
			return "e"s + others;
		else if constexpr (FirstNum == 0)
			return "x"s + others;
		else if constexpr (FirstNum == 1)
			return "y"s + others;
		else if constexpr (FirstNum == 2)
			return "z"s + others;
		else
			return "k"s + others;
	}



	template<typename Signature, int... Nums, typename... Others>
	std::string to_string(GA::Multivector<GA::BaseVector<Signature, Nums...>, Others...> const& value)
	{
		using namespace std::string_literals;

		if (value.value == 0)
		{
			if constexpr (sizeof...(Others) != 0)
				return GA::to_string(value.others);
			else
				return "0"s;
		}
		else
		{
			std::string result = std::to_string(value.value);
			if constexpr (sizeof...(Nums) != 0)
			{
				result += " "s + GetVectorSuffix< Signature, Nums...>();
			}
			if constexpr (sizeof...(Others) != 0)
			{
				result += " + "s + GA::to_string(value.others);
			}
			return result;
		}
	}

	std::string to_string(GA::Multivector<> const& value)
	{
		using namespace std::string_literals;
		return "0"s;
	}
}

namespace Catch {
	template<typename... BaseVectors>
	struct StringMaker<GA::Multivector<BaseVectors...>> {
		static std::string convert(GA::Multivector<BaseVectors...> const& value) {
			return GA::to_string(value);
		}
	};
	template<typename... BaseVectors>
	struct StringMaker<GA::Versor<BaseVectors...>> {
		static std::string convert(GA::Versor<BaseVectors...> const& value) {
			return GA::to_string(value);
		}
	};
	template<typename... BaseVectors>
	struct StringMaker<GA::Blade<BaseVectors...>> {
		static std::string convert(GA::Blade<BaseVectors...> const& value) {
			return GA::to_string(value);
		}
	};
}


TEST_CASE("2d euclidean multiplication", "[2d euclidean]")
{
	using namespace GA;
	using Euclidean2D = Signature<double, 2>;
	using Scalar = BaseVector<Euclidean2D>;
	using X = BaseVector<Euclidean2D, 0>;
	using Y = BaseVector<Euclidean2D, 1>;
	using XY = BaseVector<Euclidean2D, 0, 1>;
		
	SECTION("scalar * scalar")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(Scalar{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(Scalar{}) == 1);
		}
	}
		
	SECTION("scalar * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<X>(Scalar{}.MultType(X{})));
			REQUIRE(IsType<Y>(Scalar{}.MultType(Y{})));
			REQUIRE(IsType<X>(X{}.MultType(Scalar{})));
			REQUIRE(IsType<Y>(Y{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(X{}) == 1);
			REQUIRE(Scalar{}.MultSign(Y{}) == 1);
			REQUIRE(X{}.MultSign(Scalar{}) == 1);
			REQUIRE(Y{}.MultSign(Scalar{}) == 1);
		}
	}

	SECTION("scalar * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<XY>(Scalar{}.MultType(XY{})));
			REQUIRE(IsType<XY>(XY{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(XY{}) == 1);
			REQUIRE(XY{}.MultSign(Scalar{}) == 1);
		}
	}

	SECTION("vec * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(X{}.MultType(X{})));
			REQUIRE(IsType<Scalar>(Y{}.MultType(Y{})));
			REQUIRE(IsType<XY>(X{}.MultType(Y{})));
			REQUIRE(IsType<XY>(Y{}.MultType(X{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(X{}) == 1);
			REQUIRE(Y{}.MultSign(Y{}) == 1);
			REQUIRE(X{}.MultSign(Y{}) == 1);
			REQUIRE(Y{}.MultSign(X{}) == -1);
		}
	}

	SECTION("vec * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Y>(X{}.MultType(XY{})));
			REQUIRE(IsType<X>(Y{}.MultType(XY{})));
			REQUIRE(IsType<Y>(XY{}.MultType(X{})));
			REQUIRE(IsType<X>(XY{}.MultType(Y{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(XY{}) == 1);
			REQUIRE(Y{}.MultSign(XY{}) == -1);
			REQUIRE(XY{}.MultSign(X{}) == -1);
			REQUIRE(XY{}.MultSign(Y{}) == 1);
		}
	}

	SECTION("biv * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(XY{}.MultType(XY{})));
		}
		SECTION("sign")
		{
			REQUIRE(XY{}.MultSign(XY{}) == -1);
		}
	}
}

TEST_CASE("2d non-euclidean multiplication", "[2d non-euclidean]")
{
	using namespace GA;
	using Euclidean2D = Signature<double, 1, 1>;
	using Scalar = BaseVector<Euclidean2D>;
	using X = BaseVector<Euclidean2D, 0>;
	using N = BaseVector<Euclidean2D, 1>; // n * n = -1
	using XN = BaseVector<Euclidean2D, 0, 1>;
		
	SECTION("scalar * scalar")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(Scalar{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(Scalar{}) == 1);
		}
	}
		
	SECTION("scalar * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<X>(Scalar{}.MultType(X{})));
			REQUIRE(IsType<N>(Scalar{}.MultType(N{})));
			REQUIRE(IsType<X>(X{}.MultType(Scalar{})));
			REQUIRE(IsType<N>(N{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(X{}) == 1);
			REQUIRE(Scalar{}.MultSign(N{}) == 1);
			REQUIRE(X{}.MultSign(Scalar{}) == 1);
			REQUIRE(N{}.MultSign(Scalar{}) == 1);
		}
	}

	SECTION("scalar * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<XN>(Scalar{}.MultType(XN{})));
			REQUIRE(IsType<XN>(XN{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(XN{}) == 1);
			REQUIRE(XN{}.MultSign(Scalar{}) == 1);
		}
	}

	SECTION("vec * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(X{}.MultType(X{})));
			REQUIRE(IsType<Scalar>(N{}.MultType(N{})));
			REQUIRE(IsType<XN>(X{}.MultType(N{})));
			REQUIRE(IsType<XN>(N{}.MultType(X{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(X{}) == 1);
			REQUIRE(N{}.MultSign(N{}) == -1);
			REQUIRE(X{}.MultSign(N{}) == 1);
			REQUIRE(N{}.MultSign(X{}) == -1);
		}
	}

	SECTION("vec * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<N>(X{}.MultType(XN{})));
			REQUIRE(IsType<X>(N{}.MultType(XN{})));
			REQUIRE(IsType<N>(XN{}.MultType(X{})));
			REQUIRE(IsType<X>(XN{}.MultType(N{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(XN{}) == 1);
			REQUIRE(N{}.MultSign(XN{}) == 1);
			REQUIRE(XN{}.MultSign(X{}) == -1);
			REQUIRE(XN{}.MultSign(N{}) == -1);
		}
	}

	SECTION("biv * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(XN{}.MultType(XN{})));
		}
		SECTION("sign")
		{
			REQUIRE(XN{}.MultSign(XN{}) == 1);
		}
	}
}

TEST_CASE("3d euclidean multiplication", "[3d euclidean]")
{
	using namespace GA;
	using Euclidean3D = Signature<double, 3>;
	using Scalar = BaseVector<Euclidean3D>;
	using X = BaseVector<Euclidean3D, 0>;
	using Y = BaseVector<Euclidean3D, 1>;
	using Z = BaseVector<Euclidean3D, 2>;
	using XY = BaseVector<Euclidean3D, 0, 1>;
	using XZ = BaseVector<Euclidean3D, 0, 2>;
	using YZ = BaseVector<Euclidean3D, 1, 2>;
	using XYZ = BaseVector<Euclidean3D, 0, 1, 2>;

	SECTION("scalar * scalar")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(Scalar{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(Scalar{}) == 1);
		}
	}

	SECTION("scalar * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<X>(Scalar{}.MultType(X{})));
			REQUIRE(IsType<Y>(Scalar{}.MultType(Y{})));
			REQUIRE(IsType<Z>(Scalar{}.MultType(Z{})));

			REQUIRE(IsType<X>(X{}.MultType(Scalar{})));
			REQUIRE(IsType<Y>(Y{}.MultType(Scalar{})));
			REQUIRE(IsType<Z>(Z{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(X{}) == 1);
			REQUIRE(Scalar{}.MultSign(Y{}) == 1);
			REQUIRE(Scalar{}.MultSign(Z{}) == 1);

			REQUIRE(X{}.MultSign(Scalar{}) == 1);
			REQUIRE(Y{}.MultSign(Scalar{}) == 1);
			REQUIRE(Z{}.MultSign(Scalar{}) == 1);
		}
	}


	SECTION("scalar * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<XY>(Scalar{}.MultType(XY{})));
			REQUIRE(IsType<XZ>(Scalar{}.MultType(XZ{})));
			REQUIRE(IsType<YZ>(Scalar{}.MultType(YZ{})));

			REQUIRE(IsType<XY>(XY{}.MultType(Scalar{})));
			REQUIRE(IsType<XZ>(XZ{}.MultType(Scalar{})));
			REQUIRE(IsType<YZ>(YZ{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(XY{}) == 1);
			REQUIRE(Scalar{}.MultSign(XZ{}) == 1);
			REQUIRE(Scalar{}.MultSign(YZ{}) == 1);

			REQUIRE(XY{}.MultSign(Scalar{}) == 1);
			REQUIRE(XZ{}.MultSign(Scalar{}) == 1);
			REQUIRE(YZ{}.MultSign(Scalar{}) == 1);
		}
	}


	SECTION("scalar * tri")
	{
		SECTION("type")
		{
			REQUIRE(IsType<XYZ>(Scalar{}.MultType(XYZ{})));
			REQUIRE(IsType<XYZ>(XYZ{}.MultType(Scalar{})));
		}
		SECTION("sign")
		{
			REQUIRE(Scalar{}.MultSign(XYZ{}) == 1);
			REQUIRE(XYZ{}.MultSign(Scalar{}) == 1);
		}
	}


	SECTION("vec * vec")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(X{}.MultType(X{})));
			REQUIRE(IsType<XY>(X{}.MultType(Y{})));
			REQUIRE(IsType<XZ>(X{}.MultType(Z{})));

			REQUIRE(IsType<XY>(Y{}.MultType(X{})));
			REQUIRE(IsType<Scalar>(Y{}.MultType(Y{})));
			REQUIRE(IsType<YZ>(Y{}.MultType(Z{})));

			REQUIRE(IsType<XZ>(Z{}.MultType(X{})));
			REQUIRE(IsType<YZ>(Z{}.MultType(Y{})));
			REQUIRE(IsType<Scalar>(Z{}.MultType(Z{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(X{}) == 1);
			REQUIRE(X{}.MultSign(Y{}) == 1);
			REQUIRE(X{}.MultSign(Z{}) == 1);

			REQUIRE(Y{}.MultSign(X{}) == -1);
			REQUIRE(Y{}.MultSign(Y{}) == 1);
			REQUIRE(Y{}.MultSign(Z{}) == 1);

			REQUIRE(Z{}.MultSign(X{}) == -1);
			REQUIRE(Z{}.MultSign(Y{}) == -1);
			REQUIRE(Z{}.MultSign(Z{}) == 1);
		}
	}


	SECTION("vec * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Y>(X{}.MultType(XY{})));
			REQUIRE(IsType<Z>(X{}.MultType(XZ{})));
			REQUIRE(IsType<XYZ>(X{}.MultType(YZ{})));

			REQUIRE(IsType<X>(Y{}.MultType(XY{})));
			REQUIRE(IsType<XYZ>(Y{}.MultType(XZ{})));
			REQUIRE(IsType<Z>(Y{}.MultType(YZ{})));

			REQUIRE(IsType<XYZ>(Z{}.MultType(XY{})));
			REQUIRE(IsType<X>(Z{}.MultType(XZ{})));
			REQUIRE(IsType<Y>(Z{}.MultType(YZ{})));

			REQUIRE(IsType<Y>(XY{}.MultType(X{})));
			REQUIRE(IsType<X>(XY{}.MultType(Y{})));
			REQUIRE(IsType<XYZ>(XY{}.MultType(Z{})));

			REQUIRE(IsType<Z>(XZ{}.MultType(X{})));
			REQUIRE(IsType<XYZ>(XZ{}.MultType(Y{})));
			REQUIRE(IsType<X>(XZ{}.MultType(Z{})));

			REQUIRE(IsType<XYZ>(YZ{}.MultType(X{})));
			REQUIRE(IsType<Z>(YZ{}.MultType(Y{})));
			REQUIRE(IsType<Y>(YZ{}.MultType(Z{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(XY{}) == 1);
			REQUIRE(X{}.MultSign(XZ{}) == 1);
			REQUIRE(X{}.MultSign(YZ{}) == 1);

			REQUIRE(Y{}.MultSign(XY{}) == -1);
			REQUIRE(Y{}.MultSign(XZ{}) == -1);
			REQUIRE(Y{}.MultSign(YZ{}) == 1);

			REQUIRE(Z{}.MultSign(XY{}) == 1);
			REQUIRE(Z{}.MultSign(XZ{}) == -1);
			REQUIRE(Z{}.MultSign(YZ{}) == -1);

			REQUIRE(XY{}.MultSign(X{}) == -1);
			REQUIRE(XY{}.MultSign(Y{}) == 1);
			REQUIRE(XY{}.MultSign(Z{}) == 1);

			REQUIRE(XZ{}.MultSign(X{}) == -1);
			REQUIRE(XZ{}.MultSign(Y{}) == -1);
			REQUIRE(XZ{}.MultSign(Z{}) == 1);

			REQUIRE(YZ{}.MultSign(X{}) == 1);
			REQUIRE(YZ{}.MultSign(Y{}) == -1);
			REQUIRE(YZ{}.MultSign(Z{}) == 1);
		}
	}


	SECTION("vec * tri")
	{
		SECTION("type")
		{
			REQUIRE(IsType<YZ>(X{}.MultType(XYZ{})));
			REQUIRE(IsType<XZ>(Y{}.MultType(XYZ{})));
			REQUIRE(IsType<XY>(Z{}.MultType(XYZ{})));

			REQUIRE(IsType<YZ>(XYZ{}.MultType(X{})));
			REQUIRE(IsType<XZ>(XYZ{}.MultType(Y{})));
			REQUIRE(IsType<XY>(XYZ{}.MultType(Z{})));
		}
		SECTION("sign")
		{
			REQUIRE(X{}.MultSign(XYZ{}) == 1);
			REQUIRE(Y{}.MultSign(XYZ{}) == -1);
			REQUIRE(Z{}.MultSign(XYZ{}) == 1);

			REQUIRE(XYZ{}.MultSign(X{}) == 1);
			REQUIRE(XYZ{}.MultSign(Y{}) == -1);
			REQUIRE(XYZ{}.MultSign(Z{}) == 1);
		}
	}


	SECTION("biv * biv")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(XY{}.MultType(XY{})));
			REQUIRE(IsType<YZ>(XY{}.MultType(XZ{})));
			REQUIRE(IsType<XZ>(XY{}.MultType(YZ{})));

			REQUIRE(IsType<YZ>(XZ{}.MultType(XY{})));
			REQUIRE(IsType<Scalar>(XZ{}.MultType(XZ{})));
			REQUIRE(IsType<XY>(XZ{}.MultType(YZ{})));

			REQUIRE(IsType<XZ>(YZ{}.MultType(XY{})));
			REQUIRE(IsType<XY>(YZ{}.MultType(XZ{})));
			REQUIRE(IsType<Scalar>(YZ{}.MultType(YZ{})));
		}
		SECTION("sign")
		{
			REQUIRE(XY{}.MultSign(XY{}) == -1);
			REQUIRE(XY{}.MultSign(XZ{}) == -1);
			REQUIRE(XY{}.MultSign(YZ{}) == 1);

			REQUIRE(XZ{}.MultSign(XY{}) == 1);
			REQUIRE(XZ{}.MultSign(XZ{}) == -1);
			REQUIRE(XZ{}.MultSign(YZ{}) == -1);

			REQUIRE(YZ{}.MultSign(XY{}) == -1);
			REQUIRE(YZ{}.MultSign(XZ{}) == 1);
			REQUIRE(YZ{}.MultSign(YZ{}) == -1);
		}
	}


	SECTION("biv * tri")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Z>(XY{}.MultType(XYZ{})));
			REQUIRE(IsType<Y>(XZ{}.MultType(XYZ{})));
			REQUIRE(IsType<X>(YZ{}.MultType(XYZ{})));

			REQUIRE(IsType<Z>(XYZ{}.MultType(XY{})));
			REQUIRE(IsType<Y>(XYZ{}.MultType(XZ{})));
			REQUIRE(IsType<X>(XYZ{}.MultType(YZ{})));
		}
		SECTION("sign")
		{
			REQUIRE(XY{}.MultSign(XYZ{}) == -1);
			REQUIRE(XZ{}.MultSign(XYZ{}) == 1);
			REQUIRE(YZ{}.MultSign(XYZ{}) == -1);

			REQUIRE(XYZ{}.MultSign(XY{}) == -1);
			REQUIRE(XYZ{}.MultSign(XZ{}) == 1);
			REQUIRE(XYZ{}.MultSign(YZ{}) == -1);
		}
	}


	SECTION("tri * tri")
	{
		SECTION("type")
		{
			REQUIRE(IsType<Scalar>(XYZ{}.MultType(XYZ{})));
		}
		SECTION("sign")
		{
			REQUIRE(XYZ{}.MultSign(XYZ{}) == -1);
		}
	}
}

TEST_CASE("3D Multivectors")
{
	using namespace GA;
	using Euclidean3D = Signature<double, 3>;
	using Scalar = Blade<BaseVector<Euclidean3D>>;
	using X = Blade<BaseVector<Euclidean3D, 0>>;
	using Y = Blade < BaseVector<Euclidean3D, 1>>;
	using Z = Blade < BaseVector<Euclidean3D, 2>>;
	using XY = Blade < BaseVector<Euclidean3D, 0, 1>>;
	using XZ = Blade < BaseVector<Euclidean3D, 0, 2>>;
	using YZ = Blade < BaseVector<Euclidean3D, 1, 2>>;
	using XYZ = Blade < BaseVector<Euclidean3D, 0, 1, 2>>;

	Scalar s{ 1 };
	X x{ 1 };
	Y y{ 1 };
	Z z{ 1 };
	XY xy{ 1 };
	XZ xz{ 1 };
	YZ yz{ 1 };
	XYZ xyz{ 1 };

	SECTION("Multivector Addition")
	{
		auto v_xpx = x + x;

		auto v_xpy = x + y;
		auto v_ypx = y + x;

		auto v_xpxz = x + xz;
		auto v_xzpx = xz + x;

		auto v_xp1 = x + 1;
		auto v_1px = 1 + x;


		REQUIRE(IsType<X>(v_xpx));
		REQUIRE(v_xpx.GetFactor(x) == 2);


		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D, 0>, BaseVector<Euclidean3D, 1>>>(v_xpy));
		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D, 0>, BaseVector<Euclidean3D, 1>>>(v_ypx));
		REQUIRE(v_xpy.GetFactor(x) == 1);
		REQUIRE(v_xpy.GetFactor(y) == 1);
		REQUIRE(v_xpy == v_ypx);

		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D, 0>, BaseVector<Euclidean3D, 0, 2>>>(v_xpxz));
		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D, 0>, BaseVector<Euclidean3D, 0, 2>>>(v_xzpx));
		REQUIRE(v_xpxz.GetFactor(x) == 1);
		REQUIRE(v_xpxz.GetFactor(xz) == 1);
		REQUIRE(v_xpxz == v_xzpx);

		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D>, BaseVector<Euclidean3D, 0>>>(v_xp1));
		REQUIRE(IsType<Multivector<BaseVector<Euclidean3D>, BaseVector<Euclidean3D, 0>>>(v_1px));
		REQUIRE(v_xp1.GetFactor(x) == 1);
		REQUIRE(v_xp1.GetFactor<>() == 1);
		REQUIRE(v_xp1 == v_1px);
	}
	

	SECTION("Multivector Multiplication")
	{
		auto v_xmx = x * x;

		auto v_xmy = x * y;
		auto v_ymx = y * x;

		auto v_xmxz = x * xz;
		auto v_xzmx = xz * x;

		auto v_xm1 = x * 2;
		auto v_1mx = 2 * x;


		REQUIRE(IsType<Scalar>(v_xmx));
		REQUIRE(v_xmx.GetFactor<>() == 1);
		REQUIRE(v_xmx == 1);

		REQUIRE(IsType<XY>(v_xmy));
		REQUIRE(IsType<XY>(v_ymx));
		REQUIRE(v_xmy.GetFactor(xy) == 1);
		REQUIRE(v_ymx.GetFactor(xy) == -1);
		REQUIRE(v_xmy == -v_ymx);

		REQUIRE(IsType<Z>(v_xmxz));
		REQUIRE(IsType<Z>(v_xzmx));
		REQUIRE(v_xmxz.GetFactor(z) == 1);
		REQUIRE(v_xzmx.GetFactor(z) == -1);
		REQUIRE(v_xmxz == -v_xzmx);

		REQUIRE(IsType<X>(v_xm1));
		REQUIRE(IsType<X>(v_1mx));
		REQUIRE(v_xm1.GetFactor(x) == 2);
		REQUIRE(v_xm1 == v_1mx);



		auto v_x_p_y_m_y_p_z = (x + y) * (y + z);

		REQUIRE(v_x_p_y_m_y_p_z.GetFactor(s) == 1);
		REQUIRE(v_x_p_y_m_y_p_z.GetFactor(xy) == 1);
		REQUIRE(v_x_p_y_m_y_p_z.GetFactor(xz) == 1);
		REQUIRE(v_x_p_y_m_y_p_z.GetFactor(yz) == 1);
		REQUIRE(!decltype(v_x_p_y_m_y_p_z)::IsGuaranteedBlade);
		REQUIRE(decltype(v_x_p_y_m_y_p_z)::IsGuaranteedVersor);

	}


	SECTION("Multivector Outer Product")
	{
		auto v_xmx = OuterProduct(x, x);

		auto v_xmy = OuterProduct(x, y);
		auto v_ymx = OuterProduct(y, x);

		auto v_xmxz = OuterProduct(x, xz);
		auto v_xzmx = OuterProduct(xz, x);

		auto v_xm1 = OuterProduct(x, 2);
		auto v_1mx = OuterProduct(2, x);


		REQUIRE(v_xmx == 0);

		REQUIRE(IsType<XY>(v_xmy));
		REQUIRE(IsType<XY>(v_ymx));
		REQUIRE(v_xmy.GetFactor(xy) == 1);
		REQUIRE(v_ymx.GetFactor(xy) == -1);
		REQUIRE(v_xmy == -v_ymx);

		REQUIRE(v_xmxz == 0);
		REQUIRE(v_xmxz == -v_xzmx);

		REQUIRE(IsType<X>(v_xm1));
		REQUIRE(IsType<X>(v_1mx));
		REQUIRE(v_xm1.GetFactor(x) == 2);
		REQUIRE(v_xm1 == v_1mx);
	}


	SECTION("Versor Product")
	{
		auto versor = (x + y) * x;
		REQUIRE(versor == 1 - xy);

		auto inverseVersor = Inverse(versor);

		auto remult = versor * inverseVersor;
		REQUIRE(remult.value == 1);

		auto rotated1 = VersorProduct(versor, x);
		auto rotated2 = VersorProduct(versor, y);

		REQUIRE(rotated1 == y);
		REQUIRE(rotated2 == -x);
	}
}

TEST_CASE("3D Conformal")
{
	using namespace GA;
	using Conformal3D = Signature<double, 4, 1>;
	using Scalar = Blade<BaseVector<Conformal3D>>;
	using X = Blade<BaseVector<Conformal3D, 0>>;
	using Y = Blade < BaseVector<Conformal3D, 1>>;
	using Z = Blade < BaseVector<Conformal3D, 2>>;
	using E = Blade < BaseVector<Conformal3D, 3>>;
	using N = Blade < BaseVector<Conformal3D, 4>>;

	Scalar s{ 1 };
	X x{ 1 };
	Y y{ 1 };
	Z z{ 1 };
	E e{ 1 };
	N n{ 1 };

	auto o = 0.5 * e + 0.5 * n;
	auto inf = n - e;

	REQUIRE(o * o == 0);
	REQUIRE(inf * inf == 0);

	REQUIRE(InnerProduct(o, inf) == -1);
	REQUIRE(InnerProduct(inf, o) == -1);
	REQUIRE(InnerProduct(o, -inf) == 1);
	REQUIRE(InnerProduct(-inf, o) == 1);

	REQUIRE(OuterProduct(o, inf) == e * n);

	auto Weight = [inf](auto v) { return InnerProduct(-inf, v).GetFactor<>(); };

	REQUIRE(Weight(o) == 1);
	REQUIRE(Weight(inf) == 0);

	auto CreateConformalPoint = [o, inf](auto p) { return ProjectGrade<1>(o + p + 0.5 * p * p * inf); };
	auto NormalizeConformalPoint = [inf](auto p) { return p * Inverse(-InnerProduct(inf, p)); };

	auto cx = CreateConformalPoint(x);
	auto cx2y = CreateConformalPoint(x + 2 * y);

	REQUIRE(Weight(cx) == 1);
	REQUIRE(Weight(cx2y) == 1);

	// --- TODO: move this to it's own header -------------------------------------------------------------------

	auto CreateTranslator = [inf](auto t)
	{
		return ConditionalCastToVersor<true>(1 - 0.5 * t * inf);
	};

	auto CreateRotatorAroundTheOrigin = [](double angle, auto planeOfRotation)
	{
		return ConditionalCastToVersor<true>(std::cos(0.5 * angle) - std::sin(0.5 * angle) * Normalized(planeOfRotation));
	};

	// ----------------------------------------------------------------------------------------------------------

	SECTION("Point Pairs")
	{
		auto SeparatePointPairs = [inf](auto pointPair)
		{
			auto weight = -LeftContraction(inf, pointPair);
			auto invWeight = Inverse(weight);

			auto ppSquared = InnerProduct(pointPair, pointPair);
			double pplen = std::sqrt(ppSquared.value);

			auto p1 = (pointPair - pplen) * invWeight;
			auto p2 = (pointPair + pplen) * invWeight;

			return std::make_pair(p1, p2);
		};


		auto pointPair1 = OuterProduct(CreateConformalPoint(x + y), CreateConformalPoint(x - z));

		auto [p1, p2] = SeparatePointPairs(pointPair1);


		REQUIRE(p1 == CreateConformalPoint(x + y));
		REQUIRE(p2 == CreateConformalPoint(x - z));
	}

	SECTION("Circles")
	{
		auto circle1 = OuterProduct(CreateConformalPoint(x + y), CreateConformalPoint(x - y), CreateConformalPoint(2 * x));
		auto circle2 = OuterProduct(CreateConformalPoint(x + 2 * y), CreateConformalPoint(x - 2 * y), CreateConformalPoint(3 * x));

		REQUIRE(OuterProduct(circle1, o) == 0); // origin pertains to the circle
		REQUIRE(OuterProduct(circle1, CreateConformalPoint(-x)) != 0);

		REQUIRE(OuterProduct(circle2, o) != 0); // origin does not pertain to the circle
		REQUIRE(OuterProduct(circle2, CreateConformalPoint(-x)) == 0);
	}

	SECTION("Lines")
	{
		auto line1 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(2 * x + y), inf);
		auto line2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + y), inf);

		auto direction1 = RightContraction(LeftContraction(inf, line1), o);
		auto direction2 = RightContraction(LeftContraction(inf, line2), o);

		REQUIRE(OuterProduct(line1, o) != 0); // none go through the origin
		REQUIRE(OuterProduct(line2, o) != 0);


		REQUIRE(OuterProduct(line1, CreateConformalPoint(-y)) == 0);
		REQUIRE(OuterProduct(line2, CreateConformalPoint(-y)) != 0);

		REQUIRE(OuterProduct(line1, CreateConformalPoint(x - y)) != 0);
		REQUIRE(OuterProduct(line2, CreateConformalPoint(x - y)) == 0);

		REQUIRE(direction1 == x + y);
		REQUIRE(direction2 == y);
	}

	SECTION("Planes")
	{
		auto plane1 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + y), CreateConformalPoint(x + z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + y), CreateConformalPoint(2 * x + z), inf);

		auto direction1 = RightContraction(LeftContraction(inf, plane1), o);
		auto direction2 = RightContraction(LeftContraction(inf, plane2), o);

		REQUIRE(OuterProduct(plane1, o) != 0); // none go through the origin
		REQUIRE(OuterProduct(plane2, o) != 0);


		REQUIRE(OuterProduct(plane1, CreateConformalPoint(x + 4 * y + 5 * z)) == 0);
		REQUIRE(OuterProduct(plane2, CreateConformalPoint(x + 4 * y + 5 * z)) != 0);

		REQUIRE(OuterProduct(plane1, CreateConformalPoint(-z)) != 0);
		REQUIRE(OuterProduct(plane2, CreateConformalPoint(-z)) == 0);

		REQUIRE(direction1 == y*z);
		REQUIRE(direction2 == -x*y + y*z);
	}


	SECTION("Translation")
	{
		auto plane1 = OuterProduct(o, CreateConformalPoint(y), CreateConformalPoint(z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + z), CreateConformalPoint(x + y), inf);

		auto translator = plane2 * plane1;

		REQUIRE(Normalized(translator) == CreateTranslator(2*x));

		auto p1 = VersorProduct(translator, o);
		auto p2 = VersorProduct(translator, p1);
		auto p3 = VersorProduct(translator, p2);

		REQUIRE(p1 == CreateConformalPoint(2 * x));
		REQUIRE(p2 == CreateConformalPoint(4 * x));
		REQUIRE(p3 == CreateConformalPoint(6 * x));
	}


	SECTION("Rotation at center")
	{
		auto plane1 = OuterProduct(o, CreateConformalPoint(y), CreateConformalPoint(z), inf);
		auto plane2 = OuterProduct(o, CreateConformalPoint(x + z), CreateConformalPoint(y), inf);

		auto rotator = plane2 * plane1;

		REQUIRE(Normalized(rotator) == CreateRotatorAroundTheOrigin(3.1415926535898 * 0.5, z * x));

		auto p1 = VersorProduct(rotator, CreateConformalPoint(-x));
		auto p2 = VersorProduct(rotator, p1);
		auto p3 = VersorProduct(rotator, p2);
		auto p4 = VersorProduct(rotator, p3);

		REQUIRE(p1 == CreateConformalPoint(z));
		REQUIRE(p2 == CreateConformalPoint(x));
		REQUIRE(p3 == CreateConformalPoint(-z));
		REQUIRE(p4 == CreateConformalPoint(-x));
	}

	SECTION("Rotation at x")
	{
		auto plane1 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + y), CreateConformalPoint(x + z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(2 * x + z), CreateConformalPoint(x + y), inf);

		auto rotator = plane2 * plane1;

		// REQUIRE(Normalized(rotator) == std::sqrt(0.5) - std::sqrt(0.5) * (1 - x * inf) * z * x); // TODO comparer with an epsilon

		auto p1 = VersorProduct(rotator, o);
		auto p2 = VersorProduct(rotator, p1);
		auto p3 = VersorProduct(rotator, p2);
		auto p4 = VersorProduct(rotator, p3);

		REQUIRE(p1 == CreateConformalPoint(x + z));
		REQUIRE(p2 == CreateConformalPoint(2 * x));
		REQUIRE(p3 == CreateConformalPoint(x - z));
		REQUIRE(p4 == o);
	}

	SECTION("Rotation at 2x")
	{
		auto plane1 = OuterProduct(CreateConformalPoint(2 * x), CreateConformalPoint(2 * x + y), CreateConformalPoint(2 * x + z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(2 * x), CreateConformalPoint(3 * x + z), CreateConformalPoint(2 * x + y), inf);

		auto rotator = plane2 * plane1;

		REQUIRE(Normalized(rotator) == std::sqrt(0.5) - std::sqrt(0.5) * (1 - 2 * x * inf) * z * x);

		auto p1 = VersorProduct(rotator, o);
		auto p2 = VersorProduct(rotator, p1);
		auto p3 = VersorProduct(rotator, p2);
		auto p4 = VersorProduct(rotator, p3);

		REQUIRE(p1 == CreateConformalPoint(2 * x + 2 * z));
		REQUIRE(p2 == CreateConformalPoint(4 * x));
		REQUIRE(p3 == CreateConformalPoint(2 * x - 2 * z));
		REQUIRE(p4 == o);
	}

	SECTION("Rotation at z")
	{
		auto plane1 = OuterProduct(o, CreateConformalPoint(y), CreateConformalPoint(z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(z), CreateConformalPoint(x + y + 2 * z), CreateConformalPoint(y + z), inf);

		auto rotator = plane2 * plane1;

		REQUIRE(Normalized(rotator) == std::sqrt(0.5) - std::sqrt(0.5) * (1 - z * inf) * z * x);

		auto p1 = VersorProduct(rotator, o);
		auto p2 = VersorProduct(rotator, p1);
		auto p3 = VersorProduct(rotator, p2);
		auto p4 = VersorProduct(rotator, p3);

		REQUIRE(p1 == CreateConformalPoint(-x + z));
		REQUIRE(p2 == CreateConformalPoint(2 * z));
		REQUIRE(p3 == CreateConformalPoint(x + z));
		REQUIRE(p4 == o);
	}

	SECTION("Rotation at x + z")
	{
		auto plane1 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + y), CreateConformalPoint(x + z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x + z), CreateConformalPoint(2 * x + 2 * z), CreateConformalPoint(x + y + z), inf);

		auto rotator = plane2 * plane1;
		auto rotator2 = ConditionalCastToVersor<true>(std::sqrt(0.5) - std::sqrt(0.5) * (1 - (x + z) * inf) * z * x);

		auto str1 = to_string(rotator);
		auto str2 = to_string(rotator2);

		REQUIRE(Normalized(rotator) == rotator2);

		auto p1 = VersorProduct(rotator, o);
		auto p2 = VersorProduct(rotator, p1);
		auto p3 = VersorProduct(rotator, p2);
		auto p4 = VersorProduct(rotator, p3);

		REQUIRE(p1 == CreateConformalPoint(2 * z));
		REQUIRE(p2 == CreateConformalPoint(2 * x + 2 * z));
		REQUIRE(p3 == CreateConformalPoint(2 * x));
		REQUIRE(p4 == o);
	}


	SECTION("Scale at the origin")
	{
		auto sphere1 = OuterProduct(CreateConformalPoint(-x), CreateConformalPoint(x), CreateConformalPoint(y), CreateConformalPoint(z));
		auto sphere2 = OuterProduct(CreateConformalPoint(-2 * x), CreateConformalPoint(2 * y), CreateConformalPoint(2 * x), CreateConformalPoint(2 * z));

		auto scaler = sphere2 * sphere1;

		REQUIRE(Normalized(scaler) == std::cosh(std::log(2)) + std::sinh(std::log(2)) * OuterProduct(o, inf));

		auto p1 = NormalizeConformalPoint(VersorProduct(scaler, CreateConformalPoint(x + y + z)));
		auto p2 = NormalizeConformalPoint(VersorProduct(scaler, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(scaler, p2));

		REQUIRE(p1 == CreateConformalPoint(4 * (x + y + z)));
		REQUIRE(p2 == CreateConformalPoint(16 * (x + y + z)));
		REQUIRE(p3 == CreateConformalPoint(64 * (x + y + z)));
	}


	SECTION("Scale at x")
	{
		auto sphere1 = OuterProduct(o, CreateConformalPoint(2 * x), CreateConformalPoint(x + y), CreateConformalPoint(x + z));
		auto sphere2 = OuterProduct(CreateConformalPoint(-x), CreateConformalPoint(x + 2 * y), CreateConformalPoint(3 * x), CreateConformalPoint(x + 2 * z));

		auto scaler = sphere2 * sphere1;

		REQUIRE(Normalized(scaler) == std::cosh(std::log(2)) + std::sinh(std::log(2)) * (1 - x * inf) * OuterProduct(o, inf));

		auto p1 = NormalizeConformalPoint(VersorProduct(scaler, CreateConformalPoint(x + y + z)));
		auto p2 = NormalizeConformalPoint(VersorProduct(scaler, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(scaler, p2));

		REQUIRE(p1 == CreateConformalPoint(x + 4 * (y + z)));
		REQUIRE(p2 == CreateConformalPoint(x + 16 * (y + z)));
		REQUIRE(p3 == CreateConformalPoint(x + 64 * (y + z)));
	}


	SECTION("Scale at y")
	{
		auto sphere1 = OuterProduct(o, CreateConformalPoint(x + y), CreateConformalPoint(2 * y), CreateConformalPoint(y + z));
		auto sphere2 = OuterProduct(CreateConformalPoint(-2 * x + y), CreateConformalPoint(3 * y), CreateConformalPoint(2 * x + y), CreateConformalPoint(2 * z + y));

		auto scaler = sphere2 * sphere1;

		REQUIRE(Normalized(scaler) == std::cosh(std::log(2)) + std::sinh(std::log(2)) * (1 - y * inf) * OuterProduct(o, inf));

		auto p1 = NormalizeConformalPoint(VersorProduct(scaler, CreateConformalPoint(x + y + z)));
		auto p2 = NormalizeConformalPoint(VersorProduct(scaler, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(scaler, p2));

		REQUIRE(p1 == CreateConformalPoint(y + 4 * (x + z)));
		REQUIRE(p2 == CreateConformalPoint(y + 16 * (x + z)));
		REQUIRE(p3 == CreateConformalPoint(y + 64 * (x + z)));
	}


	SECTION("Scale at 2x - z")
	{
		auto sphere1 = OuterProduct(CreateConformalPoint(-x + 2 * x - z), CreateConformalPoint(x + 2 * x - z), CreateConformalPoint(y + 2 * x - z), CreateConformalPoint(z + 2 * x - z));
		auto sphere2 = OuterProduct(CreateConformalPoint(-2 * x + 2 * x - z), CreateConformalPoint(2 * y + 2 * x - z), CreateConformalPoint(2 * x + 2 * x - z), CreateConformalPoint(2 * z + 2 * x - z));

		auto scaler = sphere2 * sphere1;

		REQUIRE(Normalized(scaler) == std::cosh(std::log(2)) + std::sinh(std::log(2)) * (1 - (2 * x - z) * inf) * OuterProduct(o, inf));

		auto p1 = NormalizeConformalPoint(VersorProduct(scaler, CreateConformalPoint(x + y + z)));
		auto p2 = NormalizeConformalPoint(VersorProduct(scaler, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(scaler, p2));

		REQUIRE(p1 == CreateConformalPoint(x + y + z + (4 - 1) * ((x + y + z) - (2 * x - z))));
		REQUIRE(p2 == CreateConformalPoint(x + y + z + (16 - 1) * ((x + y + z) - (2 * x - z))));
		REQUIRE(p3 == CreateConformalPoint(x + y + z + (64 - 1) * ((x + y + z) - (2 * x - z))));
	}
}

