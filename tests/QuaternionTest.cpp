/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Quaternion.h"
#include <catch2/catch.hpp>

TEST_CASE("QuaternionTest_testConstructors", "[QuaternionTest]") {
	// Empty constructor
	REQUIRE(Geometry::Quaternion() == Geometry::Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	// Initializing constructor
	const Geometry::Quaternion qX(1.0f, 0.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qY(0.0f, 1.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qZ(0.0f, 0.0f, 1.0f, 90.0f);

	// Equal operator and not equal operator
	REQUIRE(qX == qX);
	REQUIRE(qX != qY);
	REQUIRE(qX != qZ);

	REQUIRE(qY == qY);
	REQUIRE(qY != qX);
	REQUIRE(qY != qZ);

	REQUIRE(qZ == qZ);
	REQUIRE(qZ != qX);
	REQUIRE(qZ != qY);

	REQUIRE(qX != Geometry::Quaternion(1.0f, 0.0f, 0.0f, 91.0f));
	REQUIRE(qY != Geometry::Quaternion(0.0f, 1.0f, 0.0f, 91.0f));
	REQUIRE(qZ != Geometry::Quaternion(0.0f, 0.0f, 1.0f, 91.0f));

	// Copy constructor
	Geometry::Quaternion qX2(qX);
	Geometry::Quaternion qY2(qY);
	Geometry::Quaternion qZ2(qZ);

	REQUIRE(qX2 == qX);
	REQUIRE(qY2 == qY);
	REQUIRE(qZ2 == qZ);

	// Assignment operator
	qX2 = qY;
	qY2 = qZ;
	qZ2 = qX;

	REQUIRE(qX2 == qY);
	REQUIRE(qY2 == qZ);
	REQUIRE(qZ2 == qX);
}

TEST_CASE("QuaternionTest_testOperators", "[QuaternionTest]") {
	const Geometry::Quaternion qX(1.0f, 0.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qY(0.0f, 1.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qZ(0.0f, 0.0f, 1.0f, 90.0f);

	// Addition operator
	REQUIRE(qX + qY == Geometry::Quaternion(1.0f, 1.0f, 0.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qY;
		REQUIRE(qSum == qX + qY);
	}
	REQUIRE(qY + qX == Geometry::Quaternion(1.0f, 1.0f, 0.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qY);
		qSum += qX;
		REQUIRE(qSum == qY + qX);
	}
	REQUIRE(qX + qZ == Geometry::Quaternion(1.0f, 0.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qZ;
		REQUIRE(qSum == qX + qZ);
	}
	REQUIRE(qZ + qX == Geometry::Quaternion(1.0f, 0.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qZ);
		qSum += qX;
		REQUIRE(qSum == qZ + qX);
	}
	REQUIRE(qY + qZ == Geometry::Quaternion(0.0f, 1.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qY);
		qSum += qZ;
		REQUIRE(qSum == qY + qZ);
	}
	REQUIRE(qZ + qY == Geometry::Quaternion(0.0f, 1.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qZ);
		qSum += qY;
		REQUIRE(qSum == qZ + qY);
	}
	REQUIRE(qX + qY + qZ == Geometry::Quaternion(1.0f, 1.0f, 1.0f, 270.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qY;
		qSum += qZ;
		REQUIRE(qSum == qX + qY + qZ);
	}

	// Subtraction operator
	REQUIRE(qX - qY == Geometry::Quaternion(1.0f, -1.0f, 0.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qX);
		qDiff -= qY;
		REQUIRE(qDiff == qX - qY);
	}
	REQUIRE(qX - qZ == Geometry::Quaternion(1.0f, 0.0f, -1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qX);
		qDiff -= qZ;
		REQUIRE(qDiff == qX - qZ);
	}
	REQUIRE(qY - qX == Geometry::Quaternion(-1.0f, 1.0f, 0.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qY);
		qDiff -= qX;
		REQUIRE(qDiff == qY - qX);
	}
	REQUIRE(qY - qZ == Geometry::Quaternion(0.0f, 1.0f, -1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qY);
		qDiff -= qZ;
		REQUIRE(qDiff == qY - qZ);
	}
	REQUIRE(qZ - qX == Geometry::Quaternion(-1.0f, 0.0f, 1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qZ);
		qDiff -= qX;
		REQUIRE(qDiff == qZ - qX);
	}
	REQUIRE(qZ - qY == Geometry::Quaternion(0.0f, -1.0f, 1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qZ);
		qDiff -= qY;
		REQUIRE(qDiff == qZ - qY);
	}

	// Multiplication with scalar
	REQUIRE(qX * 2.0f == Geometry::Quaternion(2.0f, 0.0f, 0.0f, 180.0f));
	REQUIRE(qY * 2.0f == Geometry::Quaternion(0.0f, 2.0f, 0.0f, 180.0f));
	REQUIRE(qZ * 2.0f == Geometry::Quaternion(0.0f, 0.0f, 2.0f, 180.0f));
	{
		Geometry::Quaternion qXQuad(qX);
		qXQuad *= 4.0f;
		REQUIRE(qXQuad == qX * 4.0f);
	}
	{
		Geometry::Quaternion qYQuad(qY);
		qYQuad *= 4.0f;
		REQUIRE(qYQuad == qY * 4.0f);
	}
	{
		Geometry::Quaternion qZQuad(qZ);
		qZQuad *= 4.0f;
		REQUIRE(qZQuad == qZ * 4.0f);
	}

	// Division by scalar
	REQUIRE(qX / 0.5f == Geometry::Quaternion(2.0f, 0.0f, 0.0f, 180.0f));
	REQUIRE(qY / 0.5f == Geometry::Quaternion(0.0f, 2.0f, 0.0f, 180.0f));
	REQUIRE(qZ / 0.5f == Geometry::Quaternion(0.0f, 0.0f, 2.0f, 180.0f));
	{
		Geometry::Quaternion qXQuater(qX);
		qXQuater /= 4.0f;
		REQUIRE(qXQuater == qX / 4.0f);
	}
	{
		Geometry::Quaternion qYQuater(qY);
		qYQuater /= 4.0f;
		REQUIRE(qYQuater == qY / 4.0f);
	}
	{
		Geometry::Quaternion qZQuater(qZ);
		qZQuater /= 4.0f;
		REQUIRE(qZQuater == qZ / 4.0f);
	}

	// Multiplication with quaternion
	REQUIRE(qX * qY == Geometry::Quaternion(90.0f, 90.0f, 1.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qX);
		qProd *= qY;
		REQUIRE(qProd == qX * qY);
	}
	REQUIRE(qY * qX == Geometry::Quaternion(90.0f, 90.0f, -1.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qY);
		qProd *= qX;
		REQUIRE(qProd == qY * qX);
	}
	REQUIRE(qX * qZ == Geometry::Quaternion(90.0f, -1.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qX);
		qProd *= qZ;
		REQUIRE(qProd == qX * qZ);
	}
	REQUIRE(qZ * qX == Geometry::Quaternion(90.0f, 1.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qZ);
		qProd *= qX;
		REQUIRE(qProd == qZ * qX);
	}
	REQUIRE(qZ * qY == Geometry::Quaternion(-1.0f, 90.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qZ);
		qProd *= qY;
		REQUIRE(qProd == qZ * qY);
	}
	REQUIRE(qY * qZ == Geometry::Quaternion(1.0f, 90.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qY);
		qProd *= qZ;
		REQUIRE(qProd == qY * qZ);
	}

	// Division by quaternion
	{
		Geometry::Quaternion qDiv(qX);
		qDiv /= qY;
		REQUIRE(qDiv == qX / qY);
	}
	{
		Geometry::Quaternion qDiv(qY);
		qDiv /= qX;
		REQUIRE(qDiv == qY / qX);
	}
	{
		Geometry::Quaternion qDiv(qX);
		qDiv /= qZ;
		REQUIRE(qDiv == qX / qZ);
	}
	{
		Geometry::Quaternion qDiv(qZ);
		qDiv /= qX;
		REQUIRE(qDiv == qZ / qX);
	}
	{
		Geometry::Quaternion qDiv(qZ);
		qDiv /= qY;
		REQUIRE(qDiv == qZ / qY);
	}
	{
		Geometry::Quaternion qDiv(qY);
		qDiv /= qZ;
		REQUIRE(qDiv == qY / qZ);
	}

	// Combinations
	REQUIRE(qX + qX - qX == qX);
	REQUIRE(qX + qX == qX * 2.0f);
	REQUIRE(qX + qX + qX == qX * 3.0f);
	REQUIRE(qY + qY - qY == qY);
	REQUIRE(qY + qY == qY * 2.0f);
	REQUIRE(qY + qY + qY == qY * 3.0f);
	REQUIRE(qZ + qZ - qZ == qZ);
	REQUIRE(qZ + qZ == qZ * 2.0f);
	REQUIRE(qZ + qZ + qZ == qZ * 3.0f);
}

TEST_CASE("QuaternionTest_testGetters", "[QuaternionTest]") {
	const Geometry::Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	REQUIRE(q1.x() == 1.0f);
	REQUIRE(q1.y() == 2.0f);
	REQUIRE(q1.z() == 3.0f);
	REQUIRE(q1.w() == 4.0f);

	REQUIRE(q1[0] == 1.0f);
	REQUIRE(q1[1] == 2.0f);
	REQUIRE(q1[2] == 3.0f);
	REQUIRE(q1[3] == 4.0f);

	const Geometry::Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
	REQUIRE(q2.x() == 5.0f);
	REQUIRE(q2.y() == 6.0f);
	REQUIRE(q2.z() == 7.0f);
	REQUIRE(q2.w() == 8.0f);

	REQUIRE(q2[0] == 5.0f);
	REQUIRE(q2[1] == 6.0f);
	REQUIRE(q2[2] == 7.0f);
	REQUIRE(q2[3] == 8.0f);

	const Geometry::Quaternion q3(9.0f, 10.0f, 11.0f, 12.0f);
	REQUIRE(q3.x() == 9.0f);
	REQUIRE(q3.y() == 10.0f);
	REQUIRE(q3.z() == 11.0f);
	REQUIRE(q3.w() == 12.0f);

	REQUIRE(q3[0] == 9.0f);
	REQUIRE(q3[1] == 10.0f);
	REQUIRE(q3[2] == 11.0f);
	REQUIRE(q3[3] == 12.0f);
}

TEST_CASE("QuaternionTest_testSetters", "[QuaternionTest]") {
	{
		Geometry::Quaternion q1;
		q1.set(1.0f, 2.0f, 3.0f, 4.0f);
		REQUIRE(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q1;
		q1.x() = 1.0f;
		q1.y() = 2.0f;
		q1.z() = 3.0f;
		q1.w() = 4.0f;
		REQUIRE(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q1;
		q1[0] = 1.0f;
		q1[1] = 2.0f;
		q1[2] = 3.0f;
		q1[3] = 4.0f;
		REQUIRE(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q2;
		q2.set(-5.0f, -6.0f, -7.0f, -8.0f);
		REQUIRE(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
	{
		Geometry::Quaternion q2;
		q2.x() = -5.0f;
		q2.y() = -6.0f;
		q2.z() = -7.0f;
		q2.w() = -8.0f;
		REQUIRE(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
	{
		Geometry::Quaternion q2;
		q2[0] = -5.0f;
		q2[1] = -6.0f;
		q2[2] = -7.0f;
		q2[3] = -8.0f;
		REQUIRE(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
}

TEST_CASE("QuaternionTest_testConjugate", "[QuaternionTest]") {
	const Geometry::Quaternion p(1.0f, 2.0f, 3.0f, 4.0f);
	const Geometry::Quaternion pConjugate(p.conjugate());

	REQUIRE(pConjugate == Geometry::Quaternion(-1.0f, -2.0f, -3.0f, 4.0f));
	REQUIRE(pConjugate.conjugate() == p);

	const Geometry::Quaternion q(-5.0f, -6.0f, -7.0f, -8.0f);
	const Geometry::Quaternion qConjugate(q.conjugate());

	REQUIRE(qConjugate == Geometry::Quaternion(5.0f, 6.0f, 7.0f, -8.0f));
	REQUIRE(qConjugate.conjugate() == q);

	REQUIRE((p * q).conjugate() == qConjugate * pConjugate);
	REQUIRE((q * p).conjugate() == pConjugate * qConjugate);

	REQUIRE((p - pConjugate) / 2 == Geometry::Quaternion(p.x(), p.y(), p.z(), 0.0f));
	REQUIRE((p + pConjugate) / 2 == Geometry::Quaternion(0.0f, 0.0f, 0.0f, p.w()));
	REQUIRE((q - qConjugate) / 2 == Geometry::Quaternion(q.x(), q.y(), q.z(), 0.0f));
	REQUIRE((q + qConjugate) / 2 == Geometry::Quaternion(0.0f, 0.0f, 0.0f, q.w()));
}

TEST_CASE("QuaternionTest_testNorm", "[QuaternionTest]") {
	const Geometry::Quaternion p(2.0f, 3.0f, 6.0f, 0.0f);
	const Geometry::Quaternion q(-2.0f, 4.0f, -4.0f, 8.0f);

	REQUIRE(p.length() == 7.0f);
	REQUIRE(p.length() == p.conjugate().length());

	REQUIRE(q.length() == 10.0f);
	REQUIRE(q.length() == q.conjugate().length());

	REQUIRE(q.inverse() == q.conjugate() / q.length2());
}
