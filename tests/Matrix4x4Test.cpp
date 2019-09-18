/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2013 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Matrix4x4.h"
#include "SRT.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>
#include <stdint.h>
#include <sstream>
#include <catch2/catch.hpp>
#define REQUIRE_EQUAL(a,b) REQUIRE((a) == (b))
#define REQUIRE_DOUBLES_EQUAL(a,b,e) REQUIRE((((a) <= (b) + e) && ((b) <= (a) + e)))

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

template<typename T>
using Matrix4x4 = Geometry::_Matrix4x4<T>;
using Matrix4x4i = Matrix4x4<int>;
using Matrix4x4d = Matrix4x4<double>;
template<typename T>
using SRT = Geometry::_SRT<T>;
using SRTi = SRT<int>;
using SRTd = SRT<double>;
template<typename T>
using Vec3 = Geometry::_Vec3<T>;
using Vec3i = Vec3<int>;
using Vec3d = Vec3<double>;

TEST_CASE("Matrix4x4Test_testConstructors", "[Matrix4x4Test]") {
	const int data[16] = {1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8};

	// Default constructor
	REQUIRE(Matrix4x4i() == Matrix4x4i(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
	// Copy constructor
	const Matrix4x4i matrixA(data);
	REQUIRE(Matrix4x4i(matrixA) == Matrix4x4i(1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8));
	// Construction from array
	REQUIRE(Matrix4x4i(data) == Matrix4x4i(1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8));

	// Construction from SRT
	const SRTd srt(Vec3d(2, 3, 4), Vec3d(0, 1, 0), Vec3d(0, 0, -1), 5);
	REQUIRE(Matrix4x4d(srt) == Matrix4x4d(5, 0, 0, 2, 0, 0, 5, 3, 0, -5, 0, 4, 0, 0, 0, 1));
}

template <typename _T>
static void checkTranslate(const Geometry::_Matrix4x4<_T> & m, _T x, _T y, _T z) {
	for (uint_fast8_t i = 0; i < 16; ++i) {
		switch (i) {
			case 0:
			case 5:
			case 10:
			case 15:
				// Diagonal
				REQUIRE_EQUAL(static_cast<_T>(1), m.at(i));
				break;
			case 3:
				REQUIRE_EQUAL(x, m.at(i));
				break;
			case 7:
				REQUIRE_EQUAL(y, m.at(i));
				break;
			case 11:
				REQUIRE_EQUAL(z, m.at(i));
				break;
			default:
				REQUIRE_EQUAL(static_cast<_T>(0), m.at(i));
				break;
		}
	}
}

TEST_CASE("Matrix4x4Test_testOperators", "[Matrix4x4Test]") {
	{
		Matrix4x4i m1;
		m1.setIdentity();
		REQUIRE(m1.isIdentity());

		Matrix4x4i m2;
		m2.setIdentity();
		REQUIRE(m2.isIdentity());

		REQUIRE(!(m1 + m2).isIdentity());
		REQUIRE((m1 * m2).isIdentity());
	}
	{
		Matrix4x4i m1;
		m1.translate(1, 2, 3);
		checkTranslate(m1, 1, 2, 3);

		Matrix4x4i m2;
		m2.translate(3, 2, 1);
		checkTranslate(m2, 3, 2, 1);

		checkTranslate(m1 * m2, 4, 4, 4);
	}
	{
		// "Row-major series"
		const Matrix4x4i m1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		// "Column-major series"
		const Matrix4x4i m2(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);

		Matrix4x4i m1T(m1);
		m1T.transpose();
		REQUIRE(m1T == m2);

		Matrix4x4i m2T(m2);
		m2T.transpose();
		REQUIRE(m2T == m1);

		const Matrix4x4i mLeftProduct(30, 70, 110, 150, 70, 174, 278, 382, 110, 278, 446, 614, 150, 382, 614, 846);
		REQUIRE((m1 * m2) == mLeftProduct);
		const Matrix4x4i mRightProduct(276, 304, 332, 360, 304, 336, 368, 400, 332, 368, 404, 440, 360, 400, 440, 480);
		REQUIRE((m2 * m1) == mRightProduct);
	}
}

TEST_CASE("Matrix4x4Test_testGetters", "[Matrix4x4Test]") {
	const int data[16] = {1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8};

	// Data access
	Matrix4x4i matrixNonConst(data);
	const Matrix4x4i matrixConst(data);
	for (uint_fast16_t i = 0; i < 16; ++i) {
		REQUIRE_EQUAL(matrixNonConst[i], data[i]);
		REQUIRE_EQUAL(matrixConst.at(i), data[i]);
	}
	REQUIRE(std::equal(data, data + 16, matrixConst.getData()));

	REQUIRE_EQUAL(matrixConst.det(), 0);

	// Determinant
	REQUIRE_EQUAL(Matrix4x4i().det(), 1);
	REQUIRE_EQUAL(Matrix4x4i(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0).det(), -1);
	REQUIRE_EQUAL(Matrix4x4i(0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0).det(), 1);

	// Identity
	Matrix4x4i matrixId;
	REQUIRE(matrixId.isIdentity());
	for (uint_fast16_t i = 0; i < 16; ++i) {
		matrixId[i] = 5;
		REQUIRE(!matrixId.isIdentity());
		matrixId[i] = (i % 5 == 0) ? 1 : 0;
		REQUIRE(matrixId.isIdentity());
	}
}

TEST_CASE("Matrix4x4Test_testSetters", "[Matrix4x4Test]") {
}

TEST_CASE("Matrix4x4Test_testConversion", "[Matrix4x4Test]") {
	const double epsilon = 1.0e-6;

	// Check identity matrix
	REQUIRE(Matrix4x4d().convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1).equals(Matrix4x4d().toSRT(), epsilon));

	// Check scale matrix
	// Uniform scaling (positive)
	REQUIRE(Matrix4x4d().scale(5).convertsSafelyToSRT());
	REQUIRE(
			SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5).equals(Matrix4x4d().scale(5).toSRT(), epsilon));
	REQUIRE(Matrix4x4d().scale(50).convertsSafelyToSRT());
	REQUIRE(
			SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 50).equals(Matrix4x4d().scale(50).toSRT(), epsilon));
	// Uniform scaling (negative)
	REQUIRE(Matrix4x4d().scale(-10, -10, -10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), -10)
						   .equals(Matrix4x4d().scale(-10, -10, -10).toSRT(), epsilon));
	// Non-uniform scaling (not supported)
	REQUIRE(!Matrix4x4d().scale(10, 10, 20).convertsSafelyToSRT());
	REQUIRE_THROWS_AS(Matrix4x4d().scale(10, 10, 20).toSRT(), std::domain_error);
	REQUIRE(!Matrix4x4d().scale(10, 20, 10).convertsSafelyToSRT());
	REQUIRE_THROWS_AS(Matrix4x4d().scale(10, 20, 10).toSRT(), std::domain_error);
	REQUIRE(!Matrix4x4d().scale(20, 10, 10).convertsSafelyToSRT());
	REQUIRE_THROWS_AS(Matrix4x4d().scale(20, 10, 10).toSRT(), std::domain_error);
	// Mapping to a rotation, and negative scaling
	REQUIRE(Matrix4x4d().scale(10, 10, -10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, -1, 0), -10)
						   .equals(Matrix4x4d().scale(10, 10, -10).toSRT(), epsilon));
	REQUIRE(Matrix4x4d().scale(10, -10, 10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, 1, 0), -10)
						   .equals(Matrix4x4d().scale(10, -10, 10).toSRT(), epsilon));
	REQUIRE(Matrix4x4d().scale(-10, 10, 10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, -1, 0), -10)
						   .equals(Matrix4x4d().scale(-10, 10, 10).toSRT(), epsilon));
	// Mapping to a rotation
	REQUIRE(Matrix4x4d().scale(10, -10, -10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, -1, 0), 10)
						   .equals(Matrix4x4d().scale(10, -10, -10).toSRT(), epsilon));
	REQUIRE(Matrix4x4d().scale(-10, 10, -10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, 1, 0), 10)
						   .equals(Matrix4x4d().scale(-10, 10, -10).toSRT(), epsilon));
	REQUIRE(Matrix4x4d().scale(-10, -10, 10).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, -1, 0), 10)
						   .equals(Matrix4x4d().scale(-10, -10, 10).toSRT(), epsilon));

	// Check rotation matrix
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, -1, 0, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 0, -1, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, -1).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 1, 0), Vec3d(0, 0, -1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, -1, 0, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(1, 0, 0), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(-1, 0, 0), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, -1, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(-1, 0, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(1, 0, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, -1).toSRT(), epsilon));

	// Check translation matrix
	REQUIRE(Matrix4x4d().translate(1, 2, 3).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(3, 2, 1).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(1, 0, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(-1, 0, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(0, 1, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(0, -1, 0).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(0, 0, 1).convertsSafelyToSRT());
	REQUIRE(Matrix4x4d().translate(0, 0, -1).convertsSafelyToSRT());
	REQUIRE(SRTd(Vec3d(1, 2, 3), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(1, 2, 3).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(3, 2, 1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(3, 2, 1).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(1, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(1, 0, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(-1, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(-1, 0, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 1, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 1, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, -1, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, -1, 0).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, 1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 0, 1).toSRT(), epsilon));
	REQUIRE(SRTd(Vec3d(0, 0, -1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 0, -1).toSRT(), epsilon));

	// Check combinations
	REQUIRE(Matrix4x4d().translate(1, 2, 3).scale(5).convertsSafelyToSRT()); // TS
	REQUIRE(Matrix4x4d().scale(5).translate(1, 2, 3).convertsSafelyToSRT()); // ST
	REQUIRE(Matrix4x4d().scale(5).rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT()); // SR
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).scale(5).convertsSafelyToSRT()); // RS
	REQUIRE(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT()); // TR
	REQUIRE(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).translate(1, 2, 3).convertsSafelyToSRT()); // RT
	REQUIRE(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).scale(5).convertsSafelyToSRT());
	; // TRS
	REQUIRE(SRTd(Vec3d(1, 2, 3), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5)
						   .equals(Matrix4x4d().translate(1, 2, 3).scale(5).toSRT(), epsilon)); // TS
	REQUIRE(SRTd(Vec3d(5, 10, 15), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5)
						   .equals(Matrix4x4d().scale(5).translate(1, 2, 3).toSRT(), epsilon)); // ST
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().scale(5).rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon)); // SR
	REQUIRE(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).scale(5).toSRT(), epsilon)); // RS
	REQUIRE(SRTd(Vec3d(1, 2, 3), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon)); // TR
	REQUIRE(SRTd(Vec3d(1, -3, 2), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).translate(1, 2, 3).toSRT(), epsilon)); // RT
	REQUIRE(SRTd(Vec3d(1, 2, 3), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).scale(5).toSRT(),
								   epsilon)); // TRS
}

TEST_CASE("Matrix4x4Test_testSerialization", "[Matrix4x4Test]") {
	std::stringstream stream;

	for (int_fast16_t i = -1000; i <= 1000; i += 16) {
		int data[16];
		for (int_fast16_t j = i; j < i + 16; ++j) {
			data[j - i] = j;
		}
		stream << Matrix4x4i(data) << ' ';
	}

	for (int_fast16_t i = -1000; i <= 1000; i += 16) {
		int data[16];
		for (int_fast16_t j = i; j < i + 16; ++j) {
			data[j - i] = j;
		}
		Matrix4x4i matrix;
		stream >> matrix;
		REQUIRE(std::equal(data, data + 16, matrix.getData()));
	}
}

TEST_CASE("Matrix4x4Test_testVectorTransformation", "[Matrix4x4Test]") {
	const double epsilon = 1.0e-6;

	const Vec3d unitX(1, 0, 0);
	const Vec3d unitY(0, 1, 0);
	const Vec3d unitZ(0, 0, 1);
	const Vec3d diagonal = Vec3d(1, 1, 1).getNormalized();

	const double scale = 789.0;

	// ##### Test positions #####

	// Translation moves positions
	REQUIRE(Vec3d(0, 0, 0).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(-1, -2, -3), epsilon));
	REQUIRE(Vec3d(1, 2, 3).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(0, 0, 0), epsilon));
	REQUIRE(Vec3d(4, 4, 4).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(3, 2, 1), epsilon));

	// Rotations of X position
	REQUIRE(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitX), epsilon));
	REQUIRE(Vec3d(0, 0, -1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitX), epsilon));
	REQUIRE(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitX), epsilon));

	// Rotations of Y position
	REQUIRE(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitY), epsilon));
	REQUIRE(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitY), epsilon));
	REQUIRE(Vec3d(-1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitY), epsilon));

	// Rotations of Z position
	REQUIRE(Vec3d(0, -1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitZ), epsilon));
	REQUIRE(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitZ), epsilon));
	REQUIRE(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitZ), epsilon));

	// Scale
	REQUIRE(Vec3d(unitX * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitX), epsilon));
	REQUIRE(Vec3d(unitY * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitY), epsilon));
	REQUIRE(Vec3d(unitZ * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitZ), epsilon));
	REQUIRE(Vec3d(diagonal * scale).equals(Matrix4x4d().scale(scale).transformPosition(diagonal), epsilon));

	// ##### Test directions #####

	// Translation must not change direction
	REQUIRE(unitX.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitX), epsilon));
	REQUIRE(unitY.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitY), epsilon));
	REQUIRE(unitZ.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitZ), epsilon));
	REQUIRE(diagonal.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(diagonal), epsilon));

	// Rotations of X direction
	REQUIRE(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitX), epsilon));
	REQUIRE(Vec3d(0, 0, -1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitX), epsilon));
	REQUIRE(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitX), epsilon));

	// Rotations of Y direction
	REQUIRE(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitY), epsilon));
	REQUIRE(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitY), epsilon));
	REQUIRE(Vec3d(-1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitY), epsilon));

	// Rotations of Z direction
	REQUIRE(Vec3d(0, -1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitZ), epsilon));
	REQUIRE(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitZ), epsilon));
	REQUIRE(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitZ), epsilon));

	// Scale
	REQUIRE(Vec3d(unitX * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitX), epsilon));
	REQUIRE(Vec3d(unitY * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitY), epsilon));
	REQUIRE(Vec3d(unitZ * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitZ), epsilon));
	REQUIRE(Vec3d(diagonal * scale).equals(Matrix4x4d().scale(scale).transformDirection(diagonal), epsilon));
}
