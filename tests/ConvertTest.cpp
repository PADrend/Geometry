/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "ConvertTest.h"
#include "Convert.h"
#include <limits>
#include <string>
CPPUNIT_TEST_SUITE_REGISTRATION(ConvertTest);

template <typename signed_t, typename unsigned_t, typename float_t>
static void testNormalConversionForTypes() {
	using Geometry::Convert;

	const float_t minusOne = static_cast<float_t>(-1);
	const float_t minusHalf = static_cast<float_t>(-0.5);
	const float_t zero = static_cast<float_t>(0);
	const float_t half = static_cast<float_t>(0.5);
	const float_t one = static_cast<float_t>(1);

	const signed_t sMin = static_cast<signed_t>(std::numeric_limits<signed_t>::min() + 1);
	const signed_t sZero = static_cast<signed_t>(0);
	const signed_t sMax = std::numeric_limits<signed_t>::max();
	const signed_t sMinusHalf = static_cast<signed_t>(sMin / 2);
	const signed_t sHalf = static_cast<signed_t>(sMax / 2);

	const unsigned_t uMin = static_cast<unsigned_t>(0);
	const unsigned_t uMax = std::numeric_limits<unsigned_t>::max();
	const unsigned_t uHalf = static_cast<unsigned_t>(uMax / 2);

	// signed ---> floating
	// Test invalid minimum value
	CPPUNIT_ASSERT_EQUAL(minusOne, Convert::fromSignedTo<float_t>(std::numeric_limits<signed_t>::min()));

	CPPUNIT_ASSERT_EQUAL(minusOne, Convert::fromSignedTo<float_t>(sMin));
	CPPUNIT_ASSERT_EQUAL(zero, Convert::fromSignedTo<float_t>(sZero));
	CPPUNIT_ASSERT_EQUAL(one, Convert::fromSignedTo<float_t>(sMax));

	// unsigned ---> floating
	CPPUNIT_ASSERT_EQUAL(zero, Convert::fromUnsignedTo<float_t>(uMin));
	CPPUNIT_ASSERT_EQUAL(one, Convert::fromUnsignedTo<float_t>(uMax));

	// floating ---> signed
	CPPUNIT_ASSERT_EQUAL(sMin, Convert::toSigned<signed_t>(minusOne));
	CPPUNIT_ASSERT_EQUAL(sMinusHalf, Convert::toSigned<signed_t>(minusHalf));
	CPPUNIT_ASSERT_EQUAL(sZero, Convert::toSigned<signed_t>(zero));
	CPPUNIT_ASSERT_EQUAL(sHalf, Convert::toSigned<signed_t>(half));
	CPPUNIT_ASSERT_EQUAL(sMax, Convert::toSigned<signed_t>(one));

	// floating ---> unsigned
	CPPUNIT_ASSERT_EQUAL(uMin, Convert::toUnsigned<unsigned_t>(zero));
	CPPUNIT_ASSERT_EQUAL(uHalf, Convert::toUnsigned<unsigned_t>(half));
	CPPUNIT_ASSERT_EQUAL(uMax, Convert::toUnsigned<unsigned_t>(one));

	// signed ---> floating ---> signed
	for (signed_t n = sMax; n > sMin; --n) {
		CPPUNIT_ASSERT_EQUAL(n, Convert::toSigned<signed_t>(Convert::fromSignedTo<float_t>(n)));
	}
	// unsigned ---> floating ---> unsigned
	for (unsigned_t n = uMin; n < uMax; ++n) {
		CPPUNIT_ASSERT_EQUAL(n, Convert::toUnsigned<unsigned_t>(Convert::fromUnsignedTo<float_t>(n)));
	}
}

void ConvertTest::testNormalConversion() {
	testNormalConversionForTypes<char, unsigned char, float>();
	testNormalConversionForTypes<char, unsigned char, double>();

	testNormalConversionForTypes<short, unsigned short, float>();
	testNormalConversionForTypes<short, unsigned short, double>();

	// Do not test wider integer types because of problems with floating-point precision.
}
