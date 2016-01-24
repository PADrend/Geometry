/*
	This file is part of the Geometry library.
	Copyright (C) 2011 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "QuaternionTest.h"
#include "Quaternion.h"
CPPUNIT_TEST_SUITE_REGISTRATION(QuaternionTest);

void QuaternionTest::testConstructors() {
	// Empty constructor
	CPPUNIT_ASSERT(Geometry::Quaternion() == Geometry::Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	// Initializing constructor
	const Geometry::Quaternion qX(1.0f, 0.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qY(0.0f, 1.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qZ(0.0f, 0.0f, 1.0f, 90.0f);

	// Equal operator and not equal operator
	CPPUNIT_ASSERT(qX == qX);
	CPPUNIT_ASSERT(qX != qY);
	CPPUNIT_ASSERT(qX != qZ);

	CPPUNIT_ASSERT(qY == qY);
	CPPUNIT_ASSERT(qY != qX);
	CPPUNIT_ASSERT(qY != qZ);

	CPPUNIT_ASSERT(qZ == qZ);
	CPPUNIT_ASSERT(qZ != qX);
	CPPUNIT_ASSERT(qZ != qY);

	CPPUNIT_ASSERT(qX != Geometry::Quaternion(1.0f, 0.0f, 0.0f, 91.0f));
	CPPUNIT_ASSERT(qY != Geometry::Quaternion(0.0f, 1.0f, 0.0f, 91.0f));
	CPPUNIT_ASSERT(qZ != Geometry::Quaternion(0.0f, 0.0f, 1.0f, 91.0f));

	// Copy constructor
	Geometry::Quaternion qX2(qX);
	Geometry::Quaternion qY2(qY);
	Geometry::Quaternion qZ2(qZ);

	CPPUNIT_ASSERT(qX2 == qX);
	CPPUNIT_ASSERT(qY2 == qY);
	CPPUNIT_ASSERT(qZ2 == qZ);

	// Assignment operator
	qX2 = qY;
	qY2 = qZ;
	qZ2 = qX;

	CPPUNIT_ASSERT(qX2 == qY);
	CPPUNIT_ASSERT(qY2 == qZ);
	CPPUNIT_ASSERT(qZ2 == qX);
}

void QuaternionTest::testOperators() {
	const Geometry::Quaternion qX(1.0f, 0.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qY(0.0f, 1.0f, 0.0f, 90.0f);
	const Geometry::Quaternion qZ(0.0f, 0.0f, 1.0f, 90.0f);

	// Addition operator
	CPPUNIT_ASSERT(qX + qY == Geometry::Quaternion(1.0f, 1.0f, 0.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qY;
		CPPUNIT_ASSERT(qSum == qX + qY);
	}
	CPPUNIT_ASSERT(qY + qX == Geometry::Quaternion(1.0f, 1.0f, 0.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qY);
		qSum += qX;
		CPPUNIT_ASSERT(qSum == qY + qX);
	}
	CPPUNIT_ASSERT(qX + qZ == Geometry::Quaternion(1.0f, 0.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qZ;
		CPPUNIT_ASSERT(qSum == qX + qZ);
	}
	CPPUNIT_ASSERT(qZ + qX == Geometry::Quaternion(1.0f, 0.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qZ);
		qSum += qX;
		CPPUNIT_ASSERT(qSum == qZ + qX);
	}
	CPPUNIT_ASSERT(qY + qZ == Geometry::Quaternion(0.0f, 1.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qY);
		qSum += qZ;
		CPPUNIT_ASSERT(qSum == qY + qZ);
	}
	CPPUNIT_ASSERT(qZ + qY == Geometry::Quaternion(0.0f, 1.0f, 1.0f, 180.0f));
	{
		Geometry::Quaternion qSum(qZ);
		qSum += qY;
		CPPUNIT_ASSERT(qSum == qZ + qY);
	}
	CPPUNIT_ASSERT(qX + qY + qZ == Geometry::Quaternion(1.0f, 1.0f, 1.0f, 270.0f));
	{
		Geometry::Quaternion qSum(qX);
		qSum += qY;
		qSum += qZ;
		CPPUNIT_ASSERT(qSum == qX + qY + qZ);
	}

	// Subtraction operator
	CPPUNIT_ASSERT(qX - qY == Geometry::Quaternion(1.0f, -1.0f, 0.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qX);
		qDiff -= qY;
		CPPUNIT_ASSERT(qDiff == qX - qY);
	}
	CPPUNIT_ASSERT(qX - qZ == Geometry::Quaternion(1.0f, 0.0f, -1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qX);
		qDiff -= qZ;
		CPPUNIT_ASSERT(qDiff == qX - qZ);
	}
	CPPUNIT_ASSERT(qY - qX == Geometry::Quaternion(-1.0f, 1.0f, 0.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qY);
		qDiff -= qX;
		CPPUNIT_ASSERT(qDiff == qY - qX);
	}
	CPPUNIT_ASSERT(qY - qZ == Geometry::Quaternion(0.0f, 1.0f, -1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qY);
		qDiff -= qZ;
		CPPUNIT_ASSERT(qDiff == qY - qZ);
	}
	CPPUNIT_ASSERT(qZ - qX == Geometry::Quaternion(-1.0f, 0.0f, 1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qZ);
		qDiff -= qX;
		CPPUNIT_ASSERT(qDiff == qZ - qX);
	}
	CPPUNIT_ASSERT(qZ - qY == Geometry::Quaternion(0.0f, -1.0f, 1.0f, 0.0f));
	{
		Geometry::Quaternion qDiff(qZ);
		qDiff -= qY;
		CPPUNIT_ASSERT(qDiff == qZ - qY);
	}

	// Multiplication with scalar
	CPPUNIT_ASSERT(qX * 2.0f == Geometry::Quaternion(2.0f, 0.0f, 0.0f, 180.0f));
	CPPUNIT_ASSERT(qY * 2.0f == Geometry::Quaternion(0.0f, 2.0f, 0.0f, 180.0f));
	CPPUNIT_ASSERT(qZ * 2.0f == Geometry::Quaternion(0.0f, 0.0f, 2.0f, 180.0f));
	{
		Geometry::Quaternion qXQuad(qX);
		qXQuad *= 4.0f;
		CPPUNIT_ASSERT(qXQuad == qX * 4.0f);
	}
	{
		Geometry::Quaternion qYQuad(qY);
		qYQuad *= 4.0f;
		CPPUNIT_ASSERT(qYQuad == qY * 4.0f);
	}
	{
		Geometry::Quaternion qZQuad(qZ);
		qZQuad *= 4.0f;
		CPPUNIT_ASSERT(qZQuad == qZ * 4.0f);
	}

	// Division by scalar
	CPPUNIT_ASSERT(qX / 0.5f == Geometry::Quaternion(2.0f, 0.0f, 0.0f, 180.0f));
	CPPUNIT_ASSERT(qY / 0.5f == Geometry::Quaternion(0.0f, 2.0f, 0.0f, 180.0f));
	CPPUNIT_ASSERT(qZ / 0.5f == Geometry::Quaternion(0.0f, 0.0f, 2.0f, 180.0f));
	{
		Geometry::Quaternion qXQuater(qX);
		qXQuater /= 4.0f;
		CPPUNIT_ASSERT(qXQuater == qX / 4.0f);
	}
	{
		Geometry::Quaternion qYQuater(qY);
		qYQuater /= 4.0f;
		CPPUNIT_ASSERT(qYQuater == qY / 4.0f);
	}
	{
		Geometry::Quaternion qZQuater(qZ);
		qZQuater /= 4.0f;
		CPPUNIT_ASSERT(qZQuater == qZ / 4.0f);
	}

	// Multiplication with quaternion
	CPPUNIT_ASSERT(qX * qY == Geometry::Quaternion(90.0f, 90.0f, 1.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qX);
		qProd *= qY;
		CPPUNIT_ASSERT(qProd == qX * qY);
	}
	CPPUNIT_ASSERT(qY * qX == Geometry::Quaternion(90.0f, 90.0f, -1.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qY);
		qProd *= qX;
		CPPUNIT_ASSERT(qProd == qY * qX);
	}
	CPPUNIT_ASSERT(qX * qZ == Geometry::Quaternion(90.0f, -1.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qX);
		qProd *= qZ;
		CPPUNIT_ASSERT(qProd == qX * qZ);
	}
	CPPUNIT_ASSERT(qZ * qX == Geometry::Quaternion(90.0f, 1.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qZ);
		qProd *= qX;
		CPPUNIT_ASSERT(qProd == qZ * qX);
	}
	CPPUNIT_ASSERT(qZ * qY == Geometry::Quaternion(-1.0f, 90.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qZ);
		qProd *= qY;
		CPPUNIT_ASSERT(qProd == qZ * qY);
	}
	CPPUNIT_ASSERT(qY * qZ == Geometry::Quaternion(1.0f, 90.0f, 90.0f, 8100.0f));
	{
		Geometry::Quaternion qProd(qY);
		qProd *= qZ;
		CPPUNIT_ASSERT(qProd == qY * qZ);
	}

	// Division by quaternion
	{
		Geometry::Quaternion qDiv(qX);
		qDiv /= qY;
		CPPUNIT_ASSERT(qDiv == qX / qY);
	}
	{
		Geometry::Quaternion qDiv(qY);
		qDiv /= qX;
		CPPUNIT_ASSERT(qDiv == qY / qX);
	}
	{
		Geometry::Quaternion qDiv(qX);
		qDiv /= qZ;
		CPPUNIT_ASSERT(qDiv == qX / qZ);
	}
	{
		Geometry::Quaternion qDiv(qZ);
		qDiv /= qX;
		CPPUNIT_ASSERT(qDiv == qZ / qX);
	}
	{
		Geometry::Quaternion qDiv(qZ);
		qDiv /= qY;
		CPPUNIT_ASSERT(qDiv == qZ / qY);
	}
	{
		Geometry::Quaternion qDiv(qY);
		qDiv /= qZ;
		CPPUNIT_ASSERT(qDiv == qY / qZ);
	}

	// Combinations
	CPPUNIT_ASSERT(qX + qX - qX == qX);
	CPPUNIT_ASSERT(qX + qX == qX * 2.0f);
	CPPUNIT_ASSERT(qX + qX + qX == qX * 3.0f);
	CPPUNIT_ASSERT(qY + qY - qY == qY);
	CPPUNIT_ASSERT(qY + qY == qY * 2.0f);
	CPPUNIT_ASSERT(qY + qY + qY == qY * 3.0f);
	CPPUNIT_ASSERT(qZ + qZ - qZ == qZ);
	CPPUNIT_ASSERT(qZ + qZ == qZ * 2.0f);
	CPPUNIT_ASSERT(qZ + qZ + qZ == qZ * 3.0f);
}

void QuaternionTest::testGetters() {
	const Geometry::Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	CPPUNIT_ASSERT(q1.x() == 1.0f);
	CPPUNIT_ASSERT(q1.y() == 2.0f);
	CPPUNIT_ASSERT(q1.z() == 3.0f);
	CPPUNIT_ASSERT(q1.w() == 4.0f);

	CPPUNIT_ASSERT(q1[0] == 1.0f);
	CPPUNIT_ASSERT(q1[1] == 2.0f);
	CPPUNIT_ASSERT(q1[2] == 3.0f);
	CPPUNIT_ASSERT(q1[3] == 4.0f);

	const Geometry::Quaternion q2(5.0f, 6.0f, 7.0f, 8.0f);
	CPPUNIT_ASSERT(q2.x() == 5.0f);
	CPPUNIT_ASSERT(q2.y() == 6.0f);
	CPPUNIT_ASSERT(q2.z() == 7.0f);
	CPPUNIT_ASSERT(q2.w() == 8.0f);

	CPPUNIT_ASSERT(q2[0] == 5.0f);
	CPPUNIT_ASSERT(q2[1] == 6.0f);
	CPPUNIT_ASSERT(q2[2] == 7.0f);
	CPPUNIT_ASSERT(q2[3] == 8.0f);

	const Geometry::Quaternion q3(9.0f, 10.0f, 11.0f, 12.0f);
	CPPUNIT_ASSERT(q3.x() == 9.0f);
	CPPUNIT_ASSERT(q3.y() == 10.0f);
	CPPUNIT_ASSERT(q3.z() == 11.0f);
	CPPUNIT_ASSERT(q3.w() == 12.0f);

	CPPUNIT_ASSERT(q3[0] == 9.0f);
	CPPUNIT_ASSERT(q3[1] == 10.0f);
	CPPUNIT_ASSERT(q3[2] == 11.0f);
	CPPUNIT_ASSERT(q3[3] == 12.0f);
}

void QuaternionTest::testSetters() {
	{
		Geometry::Quaternion q1;
		q1.set(1.0f, 2.0f, 3.0f, 4.0f);
		CPPUNIT_ASSERT(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q1;
		q1.x() = 1.0f;
		q1.y() = 2.0f;
		q1.z() = 3.0f;
		q1.w() = 4.0f;
		CPPUNIT_ASSERT(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q1;
		q1[0] = 1.0f;
		q1[1] = 2.0f;
		q1[2] = 3.0f;
		q1[3] = 4.0f;
		CPPUNIT_ASSERT(q1 == Geometry::Quaternion(1.0f, 2.0f, 3.0f, 4.0f));
	}
	{
		Geometry::Quaternion q2;
		q2.set(-5.0f, -6.0f, -7.0f, -8.0f);
		CPPUNIT_ASSERT(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
	{
		Geometry::Quaternion q2;
		q2.x() = -5.0f;
		q2.y() = -6.0f;
		q2.z() = -7.0f;
		q2.w() = -8.0f;
		CPPUNIT_ASSERT(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
	{
		Geometry::Quaternion q2;
		q2[0] = -5.0f;
		q2[1] = -6.0f;
		q2[2] = -7.0f;
		q2[3] = -8.0f;
		CPPUNIT_ASSERT(q2 == Geometry::Quaternion(-5.0f, -6.0f, -7.0f, -8.0f));
	}
}

void QuaternionTest::testConjugate() {
	const Geometry::Quaternion p(1.0f, 2.0f, 3.0f, 4.0f);
	const Geometry::Quaternion pConjugate(p.conjugate());

	CPPUNIT_ASSERT(pConjugate == Geometry::Quaternion(-1.0f, -2.0f, -3.0f, 4.0f));
	CPPUNIT_ASSERT(pConjugate.conjugate() == p);

	const Geometry::Quaternion q(-5.0f, -6.0f, -7.0f, -8.0f);
	const Geometry::Quaternion qConjugate(q.conjugate());

	CPPUNIT_ASSERT(qConjugate == Geometry::Quaternion(5.0f, 6.0f, 7.0f, -8.0f));
	CPPUNIT_ASSERT(qConjugate.conjugate() == q);

	CPPUNIT_ASSERT((p * q).conjugate() == qConjugate * pConjugate);
	CPPUNIT_ASSERT((q * p).conjugate() == pConjugate * qConjugate);

	CPPUNIT_ASSERT((p - pConjugate) / 2 == Geometry::Quaternion(p.x(), p.y(), p.z(), 0.0f));
	CPPUNIT_ASSERT((p + pConjugate) / 2 == Geometry::Quaternion(0.0f, 0.0f, 0.0f, p.w()));
	CPPUNIT_ASSERT((q - qConjugate) / 2 == Geometry::Quaternion(q.x(), q.y(), q.z(), 0.0f));
	CPPUNIT_ASSERT((q + qConjugate) / 2 == Geometry::Quaternion(0.0f, 0.0f, 0.0f, q.w()));
}

void QuaternionTest::testNorm() {
	const Geometry::Quaternion p(2.0f, 3.0f, 6.0f, 0.0f);
	const Geometry::Quaternion q(-2.0f, 4.0f, -4.0f, 8.0f);

	CPPUNIT_ASSERT(p.length() == 7.0f);
	CPPUNIT_ASSERT(p.length() == p.conjugate().length());

	CPPUNIT_ASSERT(q.length() == 10.0f);
	CPPUNIT_ASSERT(q.length() == q.conjugate().length());

	CPPUNIT_ASSERT(q.inverse() == q.conjugate() / q.length2());
}
