#include <GeometricAlgebra.hpp>

#include "catch.hpp"

/*

--------
p = euclidean point
P = o + p + 0.5 * p*p * inf = conformal point
P*P = 0
weight = -InnerProduct(inf, P)
P, Q = conformal points
distance = 0.5 * dot(P, Q) / (weight(P) * weight(Q))

--------------------------------

line = P ^ Q ^ inf = o ^ (q - p) ^ inf + p ^ q ^ inf
line = (o ^ direction + momentum) ^ inf
direction = inf | line
direction = vector * inf
closest point to origin = (o | line) / line

plane = P ^ Q ^ M ^ inf = o ^ (q ^ m + m ^ p + p ^ q) ^ inf + p ^ q ^ m ^ inf
line = (o ^ direction + momentum) ^ inf
direction = inf | plane
direction = bivector * inf
closest point to origin = (o | plane) / plane

--------------------------------

dual sphere = (o + c + 0.5 * (c*c - r*r) * inf)
sphere = dual sphere / I

circle = Tc[ (o + 0.5r*r) * bivectorplane ]

point pair = "

------------------------------------------

directions = euclidean * inf
tangents at origin = o * euclidean direction
tangent at a point = p ^ ((euclidean direction * inf) | p)

-------------------------------------------

translation = (1 - 0.5 * t * inf) obj (1 + 0.5 * t * inf)
e^(-0.5 * t * inf)


rotation at the origin = cos(-0.5 * angle) + sin(-0.5 * angle) * bivector
e^(-0.5 * angle * bivector)


scale at the origin e^s = cosh(0.5 * s) + sinh(0.5 * s) * o ^ inf
e^(0.5 * s * o ^ inf)

-------------------------------------------

rotation at a point = cos(-0.5 * angle) + sin(-0.5 * angle) * (bivector + (bivector | point) * inf)
e^(-0.5 * angle * (bivector + (bivector | point) * inf))

scale at a point = cosh(0.5 * s) + sinh(0.5 * s) * (o + p) ^ inf
e^(0.5s * (o + p) ^ inf)

-------------------------------------------

screw: cos(-0.5 * angle)                                                                       // scalar   (1)
     + sin(-0.5 * angle) * bivector                                                            // bivector (3)
	 + (sin(-0.5 * angle) * (bivector | point) - 0.5 * cos(-0.5 * angle) * translation) ^ inf  // vector ^ inf (3 (+3))
	 + 0.5 * sin(-0.5 * angle) * translation ^ bivector ^ inf                                  // trivector ^ inf (1 (+1))



e^(-0.5 * (angle * (bivector + (bivector | point) ^ inf) + t ^ inf))
e^(-0.5 * (angle * bivector                         // 3
         + (angle * (bivector | point) + t) ^ inf   // 3 (+3)
		 )

-------------------------------------------

spiral

spi = cos * cosh 
	+ cos * sinh * o ^ inf
    + sin * cosh * bivector
	+ sin * sinh * o ^ bivector ^ inf

e^(0.5 * (-angle * bivector + s * o ^ inf))




spi = cos * cosh                                                 // scalar (1)
	+ cos * sinh * (o + point) ^ inf                             // en + vector * inf (1 + 3 (+3))
	+ sin * cosh * (bivector + (bivector | point) * inf)         // bivector + "moment" * inf (3 + [3 (+3)])
	+ sin * sinh * (o ^ bivector + bivector ^ point) ^ inf       // quadvector (amb el bivector) + volume * inf (3 + 1 (+1))

	// vector & moment son perpendiculars pero es "confonen"
	// 12 o 16 elements



e^(0.5 * (-angle * (bivector + (bivector | point) * inf) + s * (o + point) ^ inf))
e^(0.5 * (-angle * bivector                                // bivector (3)
        + (-angle * (bivector | point) + s * point) ^ inf  // vector * inf (3 (+3))
		+ s * o ^ inf)                                     // bivector (1)
		)



----------------------------------------------------------------------------------------------

Tangent:

p: conformal point 
p ^ (p | direction)


tangent | (p * inf) = distance from the tangent point to p times tangent "length"

*/






namespace GA
{

	template<typename Signature, int FirstNum, int... Nums>
	static std::string GetVectorSuffix()
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
	static std::string to_string(GA::Multivector<GA::BaseVector<Signature, Nums...>, Others...> const& value, bool addPlusSign = false)
	{
		using namespace std::string_literals;

		if (value.value == 0)
		{
			if constexpr (sizeof...(Others) != 0)
				return GA::to_string(value.others, addPlusSign);
			else
				return addPlusSign ? ""s : "0"s;
		}
		else
		{
			std::string result;
			if(!addPlusSign)
				result = std::to_string(value.value);
			else if(value.value > 0)
				result += " + "s + std::to_string(value.value);
			else
				result += " - "s + std::to_string(-value.value);

			if constexpr (sizeof...(Nums) != 0)
			{
				result += " "s + GetVectorSuffix< Signature, Nums...>();
			}
			if constexpr (sizeof...(Others) != 0)
			{
				result += GA::to_string(value.others, true);
			}
			return result;
		}
	}

	static std::string to_string(GA::Multivector<> const& value)
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

	auto I = OuterProduct(o, x, y, z, inf);

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

		REQUIRE(direction1 == y * z);
		REQUIRE(direction2 == -x * y + y * z);
	}

	SECTION("Parametric Sphere")
	{
		double r = 5;
		auto t = 2 * x + 7 * y + 13 * z;
		auto spherePart1 = (1 - 0.5 * t * inf) * o * x * y * z * (1 + 0.5 * t * inf);
		auto spherePart2 = (1 - 0.5 * t * inf) * 0.5 * r * r * inf * x * y * z * (1 + 0.5 * t * inf);

		auto str0 = to_string(CreateConformalPoint(t) * x * y * z);

		auto str1 = to_string(spherePart1);
		auto str2 = to_string(spherePart2);
	}

	SECTION("Translation")
	{
		auto plane1 = OuterProduct(o, CreateConformalPoint(y), CreateConformalPoint(z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(x + z), CreateConformalPoint(x + y), inf);

		auto translator = plane2 * plane1;

		REQUIRE(Normalized(translator) == CreateTranslator(2 * x));

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

	SECTION("Oblique Rotation")
	{
		auto plane1 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(y + z), CreateConformalPoint(x + z), inf);
		auto plane2 = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(y + z), CreateConformalPoint(2 * x + 15 * z), inf);

		auto rotator = plane2 * plane1;

		auto line = OuterProduct(CreateConformalPoint(x), CreateConformalPoint(y + z), inf);

		auto str1 = to_string(rotator);
		auto str2 = to_string(line);
		
		auto str3 = to_string(Normalized(rotator));


		auto str4 = to_string(LeftContraction(rotator, -I) );
		auto str5 = to_string(LeftContraction(line, -I));



		auto rotationPlane = x * y - x * z - y * z;
		auto rotationPoint = x;

		auto str6 = to_string(rotationPlane);
		auto str7 = to_string(InnerProduct(rotationPlane, rotationPoint) * inf);




		REQUIRE(Normalized(rotator) == line);
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


	SECTION("Scale at any point")
	{
		auto translation = 2 * x + 3 * y + z;
		auto translatedScale = (1 - 0.5 * translation * inf) * OuterProduct(o, inf) * (1 + 0.5 * translation * inf);

		auto str1 = to_string(translatedScale);
		auto str2 = to_string(translation * inf);

	}


	SECTION("Spiral at any point")
	{
		auto translation = 2 * x + 3 * y + z;
		auto translatedScale = (1 - 0.5 * translation * inf) * OuterProduct(o, 2 * y * z + z * x + x * y, inf) * (1 + 0.5 * translation * inf);

		auto str1 = to_string(translatedScale);
		auto str2 = to_string(translation * inf);

	}

	SECTION("Spiral")
	{
		auto plane1 = OuterProduct(o, CreateConformalPoint(y), CreateConformalPoint(z), inf);
		auto plane2 = OuterProduct(o, CreateConformalPoint(x + 5 * z), CreateConformalPoint(y), inf);

		auto rotator = Normalized(plane2 * plane1);

		auto sphere1 = OuterProduct(CreateConformalPoint(-x), CreateConformalPoint(x), CreateConformalPoint(y), CreateConformalPoint(z));
		auto sphere2 = OuterProduct(CreateConformalPoint(-1.1 * x), CreateConformalPoint(1.1 * y), CreateConformalPoint(1.1 * x), CreateConformalPoint(1.1 * z));

		auto scaler = Normalized(sphere2 * sphere1);

		auto spiraler = rotator * scaler;
		//REQUIRE(rotator * scaler == scaler * rotator);

		auto p1 = NormalizeConformalPoint(VersorProduct(spiraler, CreateConformalPoint(x)));
		auto p2 = NormalizeConformalPoint(VersorProduct(spiraler, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(spiraler, p2));
		auto p4 = NormalizeConformalPoint(VersorProduct(spiraler, p3));
		auto p5 = NormalizeConformalPoint(VersorProduct(spiraler, p4));
		auto p6 = NormalizeConformalPoint(VersorProduct(spiraler, p5));
		auto p7 = NormalizeConformalPoint(VersorProduct(spiraler, p6));
		auto p8 = NormalizeConformalPoint(VersorProduct(spiraler, p7));
		auto p9 = NormalizeConformalPoint(VersorProduct(spiraler, p8));
		auto p10 = NormalizeConformalPoint(VersorProduct(spiraler, p9));

		auto str1 = to_string(rotator);
		auto str2 = to_string(scaler);
		auto str3 = to_string(spiraler);

		auto s1 = to_string(p1);
		auto s2 = to_string(p2);
		auto s3 = to_string(p3);
		auto s4 = to_string(p4);
		auto s5 = to_string(p5);
		auto s6 = to_string(p6);
		auto s7 = to_string(p7);
		auto s8 = to_string(p8);
		auto s9 = to_string(p9);
		auto s10 = to_string(p10);
	}


	SECTION("Transversion")
	{
		auto transversor = ConditionalCastToVersor<true>(1 + o * (5 * x + 3 * y));
		//REQUIRE(rotator * scaler == scaler * rotator);

		auto p1 = NormalizeConformalPoint(VersorProduct(transversor, CreateConformalPoint(10 * x + 30 * y)));
		auto p2 = NormalizeConformalPoint(VersorProduct(transversor, p1));
		auto p3 = NormalizeConformalPoint(VersorProduct(transversor, p2));
		auto p4 = NormalizeConformalPoint(VersorProduct(transversor, p3));
		auto p5 = NormalizeConformalPoint(VersorProduct(transversor, p4));
		auto p6 = NormalizeConformalPoint(VersorProduct(transversor, p5));
		auto p7 = NormalizeConformalPoint(VersorProduct(transversor, p6));
		auto p8 = NormalizeConformalPoint(VersorProduct(transversor, p7));
		auto p9 = NormalizeConformalPoint(VersorProduct(transversor, p8));
		auto p10 = NormalizeConformalPoint(VersorProduct(transversor, p9));

		auto str = to_string(transversor);

		auto s1 = to_string(p1);
		auto s2 = to_string(p2);
		auto s3 = to_string(p3);
		auto s4 = to_string(p4);
		auto s5 = to_string(p5);
		auto s6 = to_string(p6);
		auto s7 = to_string(p7);
		auto s8 = to_string(p8);
		auto s9 = to_string(p9);
		auto s10 = to_string(p10);
	}


	auto CreateTangent = [inf, CreateConformalPoint](auto point, auto direction)
	{
		auto cp = CreateConformalPoint(point);
		auto directionBivector = direction * inf;
		return OuterProduct(cp, LeftContraction(cp, directionBivector));
	};

	SECTION("Tangent")
	{
		auto tangent1 = CreateTangent(x, std::sqrt(0.5) * y - std::sqrt(0.5) * z);
		auto tangent2 = CreateTangent(x, -y + z);

		auto tangent3 = CreateTangent(x + y - z, std::sqrt(0.5) * y - std::sqrt(0.5) * z);
		auto tangent4 = CreateTangent(x + y - z, -std::sqrt(0.5) * y + std::sqrt(0.5) * z);

		auto tangent5 = CreateTangent(x - y + z, std::sqrt(0.5) * y - std::sqrt(0.5) * z);
		auto tangent6 = CreateTangent(x - y + z, -std::sqrt(0.5) * y + std::sqrt(0.5) * z);

		auto tangent7 = CreateTangent(2 * x, std::sqrt(0.5) * y - std::sqrt(0.5) * z);
		auto tangent8 = CreateTangent(2 * x, -std::sqrt(0.5) * y + std::sqrt(0.5) * z);

		auto tangent9 = CreateTangent(5 * x, std::sqrt(0.5) * y - std::sqrt(0.5) * z);
		auto tangent0 = CreateTangent(5 * x, -std::sqrt(0.5) * y + std::sqrt(0.5) * z);

		auto p1 = CreateConformalPoint(x);
		auto p2 = CreateConformalPoint(x + y - z);
		auto p3 = CreateConformalPoint(x - y + z);
		auto p4 = CreateConformalPoint(2 * x);
		auto p5 = CreateConformalPoint(-x);
		auto p6 = CreateConformalPoint(5 * x);
		auto p7 = CreateConformalPoint(x + 2 * y - 2 * z);
		auto p8 = CreateConformalPoint(x - 2 * y + 2 * z);
		auto p9 = CreateConformalPoint(4 * x + 2 * y - 2 * z);

		auto DistanceTangentToPoint = [inf](auto tangent, auto p)
		{
			return ScalarProduct(tangent, p * inf);
		};

		auto s1p1 = to_string(DistanceTangentToPoint(tangent1, p1));
		auto s1p2 = to_string(DistanceTangentToPoint(tangent1, p2));
		auto s1p3 = to_string(DistanceTangentToPoint(tangent1, p3));
		auto s1p4 = to_string(DistanceTangentToPoint(tangent1, p4));
		auto s1p5 = to_string(DistanceTangentToPoint(tangent1, p5));
		auto s1p6 = to_string(DistanceTangentToPoint(tangent1, p6));
		auto s1p7 = to_string(DistanceTangentToPoint(tangent1, p7));
		auto s1p8 = to_string(DistanceTangentToPoint(tangent1, p8));
		auto s1p9 = to_string(DistanceTangentToPoint(tangent1, p9));

		auto s2p1 = to_string(DistanceTangentToPoint(tangent2, p1));
		auto s2p2 = to_string(DistanceTangentToPoint(tangent2, p2));
		auto s2p3 = to_string(DistanceTangentToPoint(tangent2, p3));
		auto s2p4 = to_string(DistanceTangentToPoint(tangent2, p4));
		auto s2p5 = to_string(DistanceTangentToPoint(tangent2, p5));
		auto s2p6 = to_string(DistanceTangentToPoint(tangent2, p6));
		auto s2p7 = to_string(DistanceTangentToPoint(tangent2, p7));
		auto s2p8 = to_string(DistanceTangentToPoint(tangent2, p8));
		auto s2p9 = to_string(DistanceTangentToPoint(tangent2, p9));


		auto s12 = to_string(ScalarProduct(tangent1, tangent2));

		auto s13 = to_string(ScalarProduct(tangent1, tangent3));
		auto s14 = to_string(ScalarProduct(tangent1, tangent4));
		auto s15 = to_string(ScalarProduct(tangent1, tangent5));
		auto s16 = to_string(ScalarProduct(tangent1, tangent6));
		auto s17 = to_string(ScalarProduct(tangent1, tangent7));
		auto s18 = to_string(ScalarProduct(tangent1, tangent8));
		auto s19 = to_string(ScalarProduct(tangent1, tangent9));
		auto s10 = to_string(ScalarProduct(tangent1, tangent0));

		auto s23 = to_string(ScalarProduct(tangent2, tangent3));
		auto s24 = to_string(ScalarProduct(tangent2, tangent4));
		auto s25 = to_string(ScalarProduct(tangent2, tangent5));
		auto s26 = to_string(ScalarProduct(tangent2, tangent6));
		auto s27 = to_string(ScalarProduct(tangent2, tangent7));
		auto s28 = to_string(ScalarProduct(tangent2, tangent8));
		auto s29 = to_string(ScalarProduct(tangent2, tangent9));
		auto s20 = to_string(ScalarProduct(tangent2, tangent0));


		// theory: convert the tangent to a line (tangent ^ inf should do it) then make the 
		auto plane1 = OuterProduct(CreateConformalPoint(x + y - z), CreateConformalPoint(2 * x + y - z), CreateConformalPoint(x + 2 * y), inf) * std::sqrt(0.5);
		auto plane2 = OuterProduct(CreateConformalPoint(x + y - z), CreateConformalPoint(2 * x + y - z), CreateConformalPoint(x + y), inf);
		
		auto splane1 = to_string(plane1);
		auto splane2 = to_string(plane2);


		auto tangent1AsLine = to_string(OuterProduct(tangent1, inf));
		auto tangent2AsLine = to_string(OuterProduct(tangent2, inf));
		
		auto tangent1AsLine_Direction = to_string(LeftContraction(inf,OuterProduct(tangent1, inf)));
		auto tangent2AsLine_Direction = to_string(LeftContraction(inf,OuterProduct(tangent2, inf)));
		
		auto tangent1AsLine_Location = to_string(LeftContraction(o,OuterProduct(tangent1, inf)) * Inverse(OuterProduct(tangent1, inf)));
		auto tangent2AsLine_Location = to_string(LeftContraction(o,OuterProduct(tangent2, inf)) * Inverse(OuterProduct(tangent2, inf)));

		auto DistanceTangentToPlane = [o, x, y, z, inf, DistanceTangentToPoint](auto tangent, auto plane)
		{
			auto line = OuterProduct(tangent, inf);
			auto antiwedged = LeftContraction(line * OuterProduct(o, x * y * z, inf), plane);
			auto collisionPoint = RightContraction(antiwedged, o);
			auto normalizedCollisionPoint = collisionPoint * (1 / -InnerProduct(collisionPoint, inf).value);
			return DistanceTangentToPoint(tangent, normalizedCollisionPoint);
		};

		auto s1pl1 = to_string(DistanceTangentToPlane(tangent1, plane1));
		auto s2pl1 = to_string(DistanceTangentToPlane(tangent2, plane1));
		auto s1pl2 = to_string(DistanceTangentToPlane(tangent1, plane2));
		auto s2pl2 = to_string(DistanceTangentToPlane(tangent2, plane2));

	}
}
