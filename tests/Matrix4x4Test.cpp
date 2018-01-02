/*
	This file is part of the Geometry library.
	Copyright (C) 2011-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Matrix4x4Test.h"
#include "Matrix4x4.h"
#include "SRT.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>
#include <stdint.h>
CPPUNIT_TEST_SUITE_REGISTRATION(Matrix4x4Test);

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

void Matrix4x4Test::testConstructors() {
	const int data[16] = {1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8};

	// Default constructor
	CPPUNIT_ASSERT(Matrix4x4i() == Matrix4x4i(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
	// Copy constructor
	const Matrix4x4i matrixA(data);
	CPPUNIT_ASSERT(Matrix4x4i(matrixA) == Matrix4x4i(1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8));
	// Construction from array
	CPPUNIT_ASSERT(Matrix4x4i(data) == Matrix4x4i(1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8));

	// Construction from SRT
	const SRTd srt(Vec3d(2, 3, 4), Vec3d(0, 1, 0), Vec3d(0, 0, -1), 5);
	CPPUNIT_ASSERT(Matrix4x4d(srt) == Matrix4x4d(5, 0, 0, 2, 0, 0, 5, 3, 0, -5, 0, 4, 0, 0, 0, 1));
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
				CPPUNIT_ASSERT_EQUAL(static_cast<_T>(1), m.at(i));
				break;
			case 3:
				CPPUNIT_ASSERT_EQUAL(x, m.at(i));
				break;
			case 7:
				CPPUNIT_ASSERT_EQUAL(y, m.at(i));
				break;
			case 11:
				CPPUNIT_ASSERT_EQUAL(z, m.at(i));
				break;
			default:
				CPPUNIT_ASSERT_EQUAL(static_cast<_T>(0), m.at(i));
				break;
		}
	}
}

void Matrix4x4Test::testOperators() {
	{
		Matrix4x4i m1;
		m1.setIdentity();
		CPPUNIT_ASSERT(m1.isIdentity());

		Matrix4x4i m2;
		m2.setIdentity();
		CPPUNIT_ASSERT(m2.isIdentity());

		CPPUNIT_ASSERT(!(m1 + m2).isIdentity());
		CPPUNIT_ASSERT((m1 * m2).isIdentity());
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
		CPPUNIT_ASSERT(m1T == m2);

		Matrix4x4i m2T(m2);
		m2T.transpose();
		CPPUNIT_ASSERT(m2T == m1);

		const Matrix4x4i mLeftProduct(30, 70, 110, 150, 70, 174, 278, 382, 110, 278, 446, 614, 150, 382, 614, 846);
		CPPUNIT_ASSERT((m1 * m2) == mLeftProduct);
		const Matrix4x4i mRightProduct(276, 304, 332, 360, 304, 336, 368, 400, 332, 368, 404, 440, 360, 400, 440, 480);
		CPPUNIT_ASSERT((m2 * m1) == mRightProduct);
	}
}

void Matrix4x4Test::testGetters() {
	const int data[16] = {1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8};

	// Data access
	Matrix4x4i matrixNonConst(data);
	const Matrix4x4i matrixConst(data);
	for (uint_fast16_t i = 0; i < 16; ++i) {
		CPPUNIT_ASSERT_EQUAL(matrixNonConst[i], data[i]);
		CPPUNIT_ASSERT_EQUAL(matrixConst.at(i), data[i]);
	}
	CPPUNIT_ASSERT(std::equal(data, data + 16, matrixConst.getData()));

	CPPUNIT_ASSERT_EQUAL(matrixConst.det(), 0);

	// Determinant
	CPPUNIT_ASSERT_EQUAL(Matrix4x4i().det(), 1);
	CPPUNIT_ASSERT_EQUAL(Matrix4x4i(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0).det(), -1);
	CPPUNIT_ASSERT_EQUAL(Matrix4x4i(0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0).det(), 1);

	// Identity
	Matrix4x4i matrixId;
	CPPUNIT_ASSERT(matrixId.isIdentity());
	for (uint_fast16_t i = 0; i < 16; ++i) {
		matrixId[i] = 5;
		CPPUNIT_ASSERT(!matrixId.isIdentity());
		matrixId[i] = (i % 5 == 0) ? 1 : 0;
		CPPUNIT_ASSERT(matrixId.isIdentity());
	}
}

void Matrix4x4Test::testSetters() {
}

void Matrix4x4Test::testConversion() {
	const double epsilon = 1.0e-6;

	// Check identity matrix
	CPPUNIT_ASSERT(Matrix4x4d().convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1).equals(Matrix4x4d().toSRT(), epsilon));

	// Check scale matrix
	// Uniform scaling (positive)
	CPPUNIT_ASSERT(Matrix4x4d().scale(5).convertsSafelyToSRT());
	CPPUNIT_ASSERT(
			SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5).equals(Matrix4x4d().scale(5).toSRT(), epsilon));
	CPPUNIT_ASSERT(Matrix4x4d().scale(50).convertsSafelyToSRT());
	CPPUNIT_ASSERT(
			SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 50).equals(Matrix4x4d().scale(50).toSRT(), epsilon));
	// Uniform scaling (negative)
	CPPUNIT_ASSERT(Matrix4x4d().scale(-10, -10, -10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), -10)
						   .equals(Matrix4x4d().scale(-10, -10, -10).toSRT(), epsilon));
	// Non-uniform scaling (not supported)
	CPPUNIT_ASSERT(!Matrix4x4d().scale(10, 10, 20).convertsSafelyToSRT());
	CPPUNIT_ASSERT_THROW(Matrix4x4d().scale(10, 10, 20).toSRT(), std::domain_error);
	CPPUNIT_ASSERT(!Matrix4x4d().scale(10, 20, 10).convertsSafelyToSRT());
	CPPUNIT_ASSERT_THROW(Matrix4x4d().scale(10, 20, 10).toSRT(), std::domain_error);
	CPPUNIT_ASSERT(!Matrix4x4d().scale(20, 10, 10).convertsSafelyToSRT());
	CPPUNIT_ASSERT_THROW(Matrix4x4d().scale(20, 10, 10).toSRT(), std::domain_error);
	// Mapping to a rotation, and negative scaling
	CPPUNIT_ASSERT(Matrix4x4d().scale(10, 10, -10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, -1, 0), -10)
						   .equals(Matrix4x4d().scale(10, 10, -10).toSRT(), epsilon));
	CPPUNIT_ASSERT(Matrix4x4d().scale(10, -10, 10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, 1, 0), -10)
						   .equals(Matrix4x4d().scale(10, -10, 10).toSRT(), epsilon));
	CPPUNIT_ASSERT(Matrix4x4d().scale(-10, 10, 10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, -1, 0), -10)
						   .equals(Matrix4x4d().scale(-10, 10, 10).toSRT(), epsilon));
	// Mapping to a rotation
	CPPUNIT_ASSERT(Matrix4x4d().scale(10, -10, -10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, -1, 0), 10)
						   .equals(Matrix4x4d().scale(10, -10, -10).toSRT(), epsilon));
	CPPUNIT_ASSERT(Matrix4x4d().scale(-10, 10, -10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, -1), Vec3d(0, 1, 0), 10)
						   .equals(Matrix4x4d().scale(-10, 10, -10).toSRT(), epsilon));
	CPPUNIT_ASSERT(Matrix4x4d().scale(-10, -10, 10).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(0, -1, 0), 10)
						   .equals(Matrix4x4d().scale(-10, -10, 10).toSRT(), epsilon));

	// Check rotation matrix
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, -1, 0, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 0, -1, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, -1).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 1, 0), Vec3d(0, 0, -1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, -1, 0, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(1, 0, 0), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(-1, 0, 0), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, -1, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(-1, 0, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, 0, 1), Vec3d(1, 0, 0), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, -1).toSRT(), epsilon));

	// Check translation matrix
	CPPUNIT_ASSERT(Matrix4x4d().translate(1, 2, 3).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(3, 2, 1).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(1, 0, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(-1, 0, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(0, 1, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(0, -1, 0).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(0, 0, 1).convertsSafelyToSRT());
	CPPUNIT_ASSERT(Matrix4x4d().translate(0, 0, -1).convertsSafelyToSRT());
	CPPUNIT_ASSERT(SRTd(Vec3d(1, 2, 3), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(1, 2, 3).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(3, 2, 1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(3, 2, 1).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(1, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(1, 0, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(-1, 0, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(-1, 0, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 1, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 1, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, -1, 0), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, -1, 0).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 0, 1).toSRT(), epsilon));
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, -1), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 1)
						   .equals(Matrix4x4d().translate(0, 0, -1).toSRT(), epsilon));

	// Check combinations
	CPPUNIT_ASSERT(Matrix4x4d().translate(1, 2, 3).scale(5).convertsSafelyToSRT()); // TS
	CPPUNIT_ASSERT(Matrix4x4d().scale(5).translate(1, 2, 3).convertsSafelyToSRT()); // ST
	CPPUNIT_ASSERT(Matrix4x4d().scale(5).rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT()); // SR
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).scale(5).convertsSafelyToSRT()); // RS
	CPPUNIT_ASSERT(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).convertsSafelyToSRT()); // TR
	CPPUNIT_ASSERT(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).translate(1, 2, 3).convertsSafelyToSRT()); // RT
	CPPUNIT_ASSERT(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).scale(5).convertsSafelyToSRT());
	; // TRS
	CPPUNIT_ASSERT(SRTd(Vec3d(1, 2, 3), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5)
						   .equals(Matrix4x4d().translate(1, 2, 3).scale(5).toSRT(), epsilon)); // TS
	CPPUNIT_ASSERT(SRTd(Vec3d(5, 10, 15), Vec3d(0, 0, 1), Vec3d(0, 1, 0), 5)
						   .equals(Matrix4x4d().scale(5).translate(1, 2, 3).toSRT(), epsilon)); // ST
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().scale(5).rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon)); // SR
	CPPUNIT_ASSERT(SRTd(Vec3d(0, 0, 0), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).scale(5).toSRT(), epsilon)); // RS
	CPPUNIT_ASSERT(SRTd(Vec3d(1, 2, 3), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).toSRT(), epsilon)); // TR
	CPPUNIT_ASSERT(SRTd(Vec3d(1, -3, 2), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 1)
						   .equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).translate(1, 2, 3).toSRT(), epsilon)); // RT
	CPPUNIT_ASSERT(SRTd(Vec3d(1, 2, 3), Vec3d(0, -1, 0), Vec3d(0, 0, 1), 5)
						   .equals(Matrix4x4d().translate(1, 2, 3).rotate_rad(M_PI_2, 1, 0, 0).scale(5).toSRT(),
								   epsilon)); // TRS
}

void Matrix4x4Test::testSerialization() {
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
		CPPUNIT_ASSERT(std::equal(data, data + 16, matrix.getData()));
	}
}

void Matrix4x4Test::testVectorTransformation() {
	const double epsilon = 1.0e-6;

	const Vec3d unitX(1, 0, 0);
	const Vec3d unitY(0, 1, 0);
	const Vec3d unitZ(0, 0, 1);
	const Vec3d diagonal = Vec3d(1, 1, 1).getNormalized();

	const double scale = 789.0;

	// ##### Test positions #####

	// Translation moves positions
	CPPUNIT_ASSERT(Vec3d(0, 0, 0).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(-1, -2, -3), epsilon));
	CPPUNIT_ASSERT(Vec3d(1, 2, 3).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(0, 0, 0), epsilon));
	CPPUNIT_ASSERT(Vec3d(4, 4, 4).equals(Matrix4x4d().translate(1, 2, 3).transformPosition(3, 2, 1), epsilon));

	// Rotations of X position
	CPPUNIT_ASSERT(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 0, -1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitX), epsilon));

	// Rotations of Y position
	CPPUNIT_ASSERT(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(-1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitY), epsilon));

	// Rotations of Z position
	CPPUNIT_ASSERT(Vec3d(0, -1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformPosition(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformPosition(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformPosition(unitZ), epsilon));

	// Scale
	CPPUNIT_ASSERT(Vec3d(unitX * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(unitY * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(unitZ * scale).equals(Matrix4x4d().scale(scale).transformPosition(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(diagonal * scale).equals(Matrix4x4d().scale(scale).transformPosition(diagonal), epsilon));

	// ##### Test directions #####

	// Translation must not change direction
	CPPUNIT_ASSERT(unitX.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitX), epsilon));
	CPPUNIT_ASSERT(unitY.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitY), epsilon));
	CPPUNIT_ASSERT(unitZ.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(unitZ), epsilon));
	CPPUNIT_ASSERT(diagonal.equals(Matrix4x4d().translate(1, 2, 3).transformDirection(diagonal), epsilon));

	// Rotations of X direction
	CPPUNIT_ASSERT(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 0, -1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitX), epsilon));

	// Rotations of Y direction
	CPPUNIT_ASSERT(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(-1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitY), epsilon));

	// Rotations of Z direction
	CPPUNIT_ASSERT(Vec3d(0, -1, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 1, 0, 0).transformDirection(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(1, 0, 0).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 1, 0).transformDirection(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(0, 0, 1).equals(Matrix4x4d().rotate_rad(M_PI_2, 0, 0, 1).transformDirection(unitZ), epsilon));

	// Scale
	CPPUNIT_ASSERT(Vec3d(unitX * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitX), epsilon));
	CPPUNIT_ASSERT(Vec3d(unitY * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitY), epsilon));
	CPPUNIT_ASSERT(Vec3d(unitZ * scale).equals(Matrix4x4d().scale(scale).transformDirection(unitZ), epsilon));
	CPPUNIT_ASSERT(Vec3d(diagonal * scale).equals(Matrix4x4d().scale(scale).transformDirection(diagonal), epsilon));
}
