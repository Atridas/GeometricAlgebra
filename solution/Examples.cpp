// GeometricAlgebra.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <type_traits>

#include <GeometricAlgebra.hpp>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

template<typename T, typename Q>
bool IsType(Q q)
{
	return std::is_same_v<T, Q>;
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
	using N = BaseVector<Euclidean2D, 1>;
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



