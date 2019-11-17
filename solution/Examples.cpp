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


namespace test2D {
	using namespace GA;
	using Euclidean2D = Signature<double, 3>;
	using Scalar = BaseVector<Euclidean2D>;
	using X = BaseVector<Euclidean2D, 0>;
	using Y = BaseVector<Euclidean2D, 1>;
	using XY = BaseVector<Euclidean2D, 0, 1>;

	TEST_CASE("2d euclidean multiplication type", "[2d euclidean]") {
		// scalar * vec
		REQUIRE(IsType<X>(Scalar{}.MultType(X{})));
		REQUIRE(IsType<Y>(Scalar{}.MultType(Y{})));
		REQUIRE(IsType<X>(X{}.MultType(Scalar{})));
		REQUIRE(IsType<Y>(Y{}.MultType(Scalar{})));

		// scalar * biv
		REQUIRE(IsType<XY>(Scalar{}.MultType(XY{})));
		REQUIRE(IsType<XY>(XY{}.MultType(Scalar{})));

		// vec * vec
		REQUIRE(IsType<Scalar>(X{}.MultType(X{})));
		REQUIRE(IsType<Scalar>(Y{}.MultType(Y{})));
		REQUIRE(IsType<XY>(X{}.MultType(Y{})));
		REQUIRE(IsType<XY>(Y{}.MultType(X{})));

		// vec * biv
		REQUIRE(IsType<Y>(X{}.MultType(XY{})));
		REQUIRE(IsType<X>(Y{}.MultType(XY{})));
		REQUIRE(IsType<Y>(XY{}.MultType(X{})));
		REQUIRE(IsType<X>(XY{}.MultType(Y{})));

		// biv * biv
		REQUIRE(IsType<Scalar>(XY{}.MultType(XY{})));
	}

	TEST_CASE("2d euclidean multiplication sign", "[2d euclidean]") {
		// scalar * vec
		REQUIRE(Scalar{}.MultSign(X{}) == 1);
		REQUIRE(Scalar{}.MultSign(Y{}) == 1);
		REQUIRE(X{}.MultSign(Scalar{}) == 1);
		REQUIRE(Y{}.MultSign(Scalar{}) == 1);

		// scalar * biv
		REQUIRE(Scalar{}.MultSign(XY{}) == 1);
		REQUIRE(XY{}.MultSign(Scalar{}) == 1);

		// vec * vec
		REQUIRE(X{}.MultSign(X{}) == 1);
		REQUIRE(Y{}.MultSign(Y{}) == 1);
		REQUIRE(X{}.MultSign(Y{}) == 1);
		REQUIRE(Y{}.MultSign(X{}) == -1);

		// vec * biv
		REQUIRE(X{}.MultSign(XY{}) == 1);
		REQUIRE(Y{}.MultSign(XY{}) == -1);
		REQUIRE(XY{}.MultSign(X{}) == -1);
		REQUIRE(XY{}.MultSign(Y{}) == 1);

		// biv * biv
		REQUIRE(XY{}.MultSign(XY{}) == -1);
	}
}


namespace test3D {
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

	TEST_CASE("3d euclidean multiplication type", "[3d euclidean]") {
		// scalar * vec
		REQUIRE(IsType<X>(Scalar{}.MultType(X{})));
		REQUIRE(IsType<Y>(Scalar{}.MultType(Y{})));
		REQUIRE(IsType<Z>(Scalar{}.MultType(Z{})));

		REQUIRE(IsType<X>(X{}.MultType(Scalar{})));
		REQUIRE(IsType<Y>(Y{}.MultType(Scalar{})));
		REQUIRE(IsType<Z>(Z{}.MultType(Scalar{})));


		// scalar * biv
		REQUIRE(IsType<XY>(Scalar{}.MultType(XY{})));
		REQUIRE(IsType<XZ>(Scalar{}.MultType(XZ{})));
		REQUIRE(IsType<YZ>(Scalar{}.MultType(YZ{})));

		REQUIRE(IsType<XY>(XY{}.MultType(Scalar{})));
		REQUIRE(IsType<XZ>(XZ{}.MultType(Scalar{})));
		REQUIRE(IsType<YZ>(YZ{}.MultType(Scalar{})));


		// scalar * tri
		REQUIRE(IsType<XYZ>(Scalar{}.MultType(XYZ{})));
		REQUIRE(IsType<XYZ>(XYZ{}.MultType(Scalar{})));


		// vec * vec
		REQUIRE(IsType<Scalar>(X{}.MultType(X{})));
		REQUIRE(IsType<XY>(X{}.MultType(Y{})));
		REQUIRE(IsType<XZ>(X{}.MultType(Z{})));

		REQUIRE(IsType<XY>(Y{}.MultType(X{})));
		REQUIRE(IsType<Scalar>(Y{}.MultType(Y{})));
		REQUIRE(IsType<YZ>(Y{}.MultType(Z{})));

		REQUIRE(IsType<XZ>(Z{}.MultType(X{})));
		REQUIRE(IsType<YZ>(Z{}.MultType(Y{})));
		REQUIRE(IsType<Scalar>(Z{}.MultType(Z{})));


		// vec * biv
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


		// vec * tri
		REQUIRE(IsType<YZ>(X{}.MultType(XYZ{})));
		REQUIRE(IsType<XZ>(Y{}.MultType(XYZ{})));
		REQUIRE(IsType<XY>(Z{}.MultType(XYZ{})));

		REQUIRE(IsType<YZ>(XYZ{}.MultType(X{})));
		REQUIRE(IsType<XZ>(XYZ{}.MultType(Y{})));
		REQUIRE(IsType<XY>(XYZ{}.MultType(Z{})));


		// biv * biv
		REQUIRE(IsType<Scalar>(XY{}.MultType(XY{})));
		REQUIRE(IsType<YZ>(XY{}.MultType(XZ{})));
		REQUIRE(IsType<XZ>(XY{}.MultType(YZ{})));

		REQUIRE(IsType<YZ>(XZ{}.MultType(XY{})));
		REQUIRE(IsType<Scalar>(XZ{}.MultType(XZ{})));
		REQUIRE(IsType<XY>(XZ{}.MultType(YZ{})));

		REQUIRE(IsType<XZ>(YZ{}.MultType(XY{})));
		REQUIRE(IsType<XY>(YZ{}.MultType(XZ{})));
		REQUIRE(IsType<Scalar>(YZ{}.MultType(YZ{})));


		// biv * tri
		REQUIRE(IsType<Z>(XY{}.MultType(XYZ{})));
		REQUIRE(IsType<Y>(XZ{}.MultType(XYZ{})));
		REQUIRE(IsType<X>(YZ{}.MultType(XYZ{})));

		REQUIRE(IsType<Z>(XYZ{}.MultType(XY{})));
		REQUIRE(IsType<Y>(XYZ{}.MultType(XZ{})));
		REQUIRE(IsType<X>(XYZ{}.MultType(YZ{})));


		// tri * tri
		REQUIRE(IsType<Scalar>(XYZ{}.MultType(XYZ{})));
	}

	TEST_CASE("3d euclidean multiplication sign", "[3d euclidean]") {
		// scalar * vec
		REQUIRE(Scalar{}.MultSign(X{}) == 1);
		REQUIRE(Scalar{}.MultSign(Y{}) == 1);
		REQUIRE(Scalar{}.MultSign(Z{}) == 1);

		REQUIRE(X{}.MultSign(Scalar{}) == 1);
		REQUIRE(Y{}.MultSign(Scalar{}) == 1);
		REQUIRE(Z{}.MultSign(Scalar{}) == 1);


		// scalar * biv
		REQUIRE(Scalar{}.MultSign(XY{}) == 1);
		REQUIRE(Scalar{}.MultSign(XZ{}) == 1);
		REQUIRE(Scalar{}.MultSign(YZ{}) == 1);

		REQUIRE(XY{}.MultSign(Scalar{}) == 1);
		REQUIRE(XZ{}.MultSign(Scalar{}) == 1);
		REQUIRE(YZ{}.MultSign(Scalar{}) == 1);


		// scalar * tri
		REQUIRE(Scalar{}.MultSign(XYZ{}) == 1);
		REQUIRE(XYZ{}.MultSign(Scalar{}) == 1);


		// vec * vec
		REQUIRE(X{}.MultSign(X{}) == 1);
		REQUIRE(X{}.MultSign(Y{}) == 1);
		REQUIRE(X{}.MultSign(Z{}) == 1);

		REQUIRE(Y{}.MultSign(X{}) == -1);
		REQUIRE(Y{}.MultSign(Y{}) == 1);
		REQUIRE(Y{}.MultSign(Z{}) == 1);

		REQUIRE(Z{}.MultSign(X{}) == -1);
		REQUIRE(Z{}.MultSign(Y{}) == -1);
		REQUIRE(Z{}.MultSign(Z{}) == 1);


		// vec * biv
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


		// vec * tri
		REQUIRE(X{}.MultSign(XYZ{}) == 1);
		REQUIRE(Y{}.MultSign(XYZ{}) == -1);
		REQUIRE(Z{}.MultSign(XYZ{}) == 1);

		REQUIRE(XYZ{}.MultSign(X{}) == 1);
		REQUIRE(XYZ{}.MultSign(Y{}) == -1);
		REQUIRE(XYZ{}.MultSign(Z{}) == 1);


		// biv * biv
		REQUIRE(XY{}.MultSign(XY{}) == -1);
		REQUIRE(XY{}.MultSign(XZ{}) == -1);
		REQUIRE(XY{}.MultSign(YZ{}) == 1);

		REQUIRE(XZ{}.MultSign(XY{}) == 1);
		REQUIRE(XZ{}.MultSign(XZ{}) == -1);
		REQUIRE(XZ{}.MultSign(YZ{}) == -1);

		REQUIRE(YZ{}.MultSign(XY{}) == -1);
		REQUIRE(YZ{}.MultSign(XZ{}) == 1);
		REQUIRE(YZ{}.MultSign(YZ{}) == -1);


		// biv * tri
		REQUIRE(XY{}.MultSign(XYZ{}) == -1);
		REQUIRE(XZ{}.MultSign(XYZ{}) == 1);
		REQUIRE(YZ{}.MultSign(XYZ{}) == -1);

		REQUIRE(XYZ{}.MultSign(XY{}) == -1);
		REQUIRE(XYZ{}.MultSign(XZ{}) == 1);
		REQUIRE(XYZ{}.MultSign(YZ{}) == -1);


		// tri * tri
		REQUIRE(XYZ{}.MultSign(XYZ{}) == -1);
	}
}

