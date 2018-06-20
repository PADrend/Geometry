/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_CONVERT_H
#define GEOMETRY_CONVERT_H

#include <algorithm>
#include <limits>
#include <type_traits>
#include <cstdint>

namespace Geometry {

struct Convert {
	/**
	 * Convert a signed normalized fixed-point value to a floating-point value.
	 *
	 * @tparam float_t Floating-point type (e.g. double)
	 * @tparam signed_t Signed normalized fixed-point type (e.g. char)
	 * @return Floating-point value in the range [-1, 1].
	 * @see Equation 2.2 in OpenGL 4.3 Core Profile Specification
	 * (updated February 14, 2013)
	 * http://www.opengl.org/registry/doc/glspec43.core.20130214.pdf
	 */
	template <typename float_t, typename signed_t,
			  typename = typename std::enable_if<std::is_integral<signed_t>::value && std::is_signed<signed_t>::value
												 && std::is_floating_point<float_t>::value>::type>
	static float_t fromSignedTo(signed_t c) {
		// max() is equal to 2^(b-1) - 1 for signed types with b bits
		const float_t f = c / static_cast<float_t>(std::numeric_limits<signed_t>::max());
		return std::min(std::max(f, static_cast<float_t>(-1)), static_cast<float_t>(1));
	}

	/**
	 * Convert an unsigned normalized fixed-point value to a floating-point
	 * value.
	 *
	 * @tparam float_t Floating-point type (e.g. double)
	 * @tparam unsigned_t Unsigned normalized fixed-point type (e.g.
	 * unsigned char)
	 * @return Floating-point value in the range [0, 1].
	 * @see Equation 2.1 in OpenGL 4.3 Core Profile Specification
	 * (updated February 14, 2013)
	 * http://www.opengl.org/registry/doc/glspec43.core.20130214.pdf
	 */
	template <typename float_t, typename unsigned_t,
			  typename =
					  typename std::enable_if<std::is_integral<unsigned_t>::value && std::is_unsigned<unsigned_t>::value
											  && std::is_floating_point<float_t>::value>::type>
	static float_t fromUnsignedTo(unsigned_t c) {
		// max() is equal to 2^b - 1 for unsigned types with b bits
		const float_t f = c / static_cast<float_t>(std::numeric_limits<unsigned_t>::max());
		return std::min(f, static_cast<float_t>(1));
	}

	/**
	 * Convert a floating-point value to a signed normalized fixed-point value.
	 *
	 * @tparam signed_t Signed normalized fixed-point type (e.g. char)
	 * @tparam float_t Floating-point type (e.g. double)
	 * @param f Floating-point value that is clamped to the range [-1, 1].
	 * @see Equation 2.4 in OpenGL 4.3 Core Profile Specification
	 * (updated February 14, 2013)
	 * http://www.opengl.org/registry/doc/glspec43.core.20130214.pdf
	 */
	template <typename signed_t, typename float_t,
			  typename = typename std::enable_if<std::is_integral<signed_t>::value && std::is_signed<signed_t>::value
												 && std::is_floating_point<float_t>::value>::type>
	static signed_t toSigned(float_t f) {
		const float_t fC = std::min(std::max(static_cast<float_t>(-1), f), static_cast<float_t>(1));
		// max() is equal to 2^(b-1) - 1 for signed types with b bits
		return fC * std::numeric_limits<signed_t>::max();
	}

	/**
	 * Convert a floating-point value to an unsigned normalized fixed-point
	 * value.
	 *
	 * @tparam unsigned_t Unsigned normalized fixed-point type (e.g.
	 * unsigned char)
	 * @tparam float_t Floating-point type (e.g. double)
	 * @param f Floating-point value that is clamped to the range [0, 1].
	 * @see Equation 2.3 in OpenGL 4.3 Core Profile Specification
	 * (updated February 14, 2013)
	 * http://www.opengl.org/registry/doc/glspec43.core.20130214.pdf
	 */
	template <typename unsigned_t, typename float_t,
			  typename =
					  typename std::enable_if<std::is_integral<unsigned_t>::value && std::is_unsigned<unsigned_t>::value
											  && std::is_floating_point<float_t>::value>::type>
	static unsigned_t toUnsigned(float_t f) {
		const float_t fC = std::min(std::max(static_cast<float_t>(0), f), static_cast<float_t>(1));
		// max() is equal to 2^b - 1 for unsigned types with b bits
		return fC * std::numeric_limits<unsigned_t>::max();
	}

	template <typename _T>
	static _T degToRad(_T f) {
		static_assert(std::is_floating_point<_T>::value, "template argument not a floating point type");
		const _T DEG_TO_RAD = 0.017453292519943295769236907684886;
		return f * DEG_TO_RAD;
	}

	template <typename _T>
	static _T radToDeg(_T f) {
		static_assert(std::is_floating_point<_T>::value, "template argument not a floating point type");
		const _T RAD_TO_DEG = 57.295779513082320876798154814105170;
		return f * RAD_TO_DEG;
	}
	
	/**
	 * Convert a 32-bit floating-point value to 16-bit floating-point value encoded in a 16-bit unsigned integer.
	 *
	 * @param value Floating-point value
	 * @return Floating-point value as uint16_t.
	 * based on 'https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion'
	 */
	static uint16_t floatToHalf(float value) {
		static int32_t const infN = 0x7F800000; // flt32 infinity
		static int32_t const maxN = 0x477FE000; // max flt16 normal as a flt32
		static int32_t const minN = 0x38800000; // min flt16 normal as a flt32
		static int32_t const nanN = 0x7f802000; // minimum flt16 nan as a flt32
		union {
			float f;
			int32_t si;
			uint32_t ui;
		} v, s;
		v.f = value;
		uint32_t sign = v.si & 0x80000000; // flt32 sign bit
		v.si ^= sign;
		sign >>= 16; // logical shift
		s.si = 0x52000000; // (1 << 23) / minN
		s.si = s.f * v.f; // correct subnormals
		v.si ^= (s.si ^ v.si) & -(minN > v.si);
		v.si ^= (infN ^ v.si) & -((infN > v.si) & (v.si > maxN));
		v.si ^= (nanN ^ v.si) & -((nanN > v.si) & (v.si > infN));
		v.ui >>= 13; // logical shift
		v.si ^= ((v.si - 0x1c000) ^ v.si) & -(v.si > 0x23bff);
		v.si ^= ((v.si - 0x1c000) ^ v.si) & -(v.si > 0x003ff);
		return v.ui | sign;
	}
	
	/**
	 * Convert a 16-bit floating-point value encoded in a 16-bit unsigned integer to a 32-bit floating-point value.
	 *
	 * @param value Floating-point value as uint16_t
	 * @return Floating-point value.
	 * based on 'https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion'
	 */
	static float halfToFloat(uint16_t value) {
		union {
			float f;
			int32_t si;
			uint32_t ui;
		} v, s;
		v.ui = value;
		int32_t sign = v.si & 0xffff8000; // flt16 sign bit
		v.si ^= sign;
		sign <<= 16;
		v.si ^= ((v.si + 0x1c000) ^ v.si) & -(v.si > 0x003ff);
		v.si ^= ((v.si + 0x1c000) ^ v.si) & -(v.si > 0x23bff);
		s.si = 0x33800000; // minN / (1 << (23 - shift))
		s.f *= v.si;
		int32_t mask = -(0x00400 > v.si);
		v.si <<= 13;
		v.si ^= (s.si ^ v.si) & mask;
		v.si |= sign;
		return v.f;
	}
};
}

#endif /* GEOMETRY_CONVERT_H */
