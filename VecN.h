/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_VECN_H
#define GEOMETRY_VECN_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace Geometry {

/*!
 * n-dimensional vector.
 *
 * @author Ralf Petring, Benjamin Eikel
 * @date 2011-02-24
 */
template <typename Type = float, typename FloatType = float>
class _VecN {
public:
	typedef Type value_t;
	typedef FloatType float_t;
	typedef _VecN<value_t, float_t> vecN_t;

	/*!
	 * comparator class for comparing vectors when using as key in a map or set
	 * comparison is done by values only not by pointers so different vectors with same values are equal
	 */
	class Comparator {
	public:
		bool operator()(const vecN_t & a, const vecN_t & b) const {
			if (a.size() != b.size())
				return a.size() < b.size();

			const value_t * vecA = a.data();
			const value_t * vecB = b.data();
			const value_t * vecA_end = vecA + a.size();
			while (vecA != vecA_end) {
				if (*vecA != *vecB)
					return *vecA < *vecB;
				vecA++;
				vecB++;
			}
			return false;
		}

		bool operator()(const vecN_t * a, const vecN_t * b) const {
			return (*this)(*a, *b);
		}
	};

private:
	std::vector<value_t> vec;

	typedef typename std::vector<value_t>::iterator iterator;
	typedef typename std::vector<value_t>::const_iterator const_iterator;

	void checkRange(size_t i) const {
		if (i >= vec.size())
			throw std::out_of_range("VecN: index out of range");
	}

	void checkSize(const _VecN & other) const {
		if (other.vec.size() != vec.size())
			throw std::range_error("VecN: incompatible size");
	}

	void checkSize(const size_t _size) const {
		if (_size == 0)
			throw std::range_error("VecN: tried to create a zero dimensional vector ");
	}

	template <class T>
	struct Sum {
		T & result;
		Sum(T & _result) : result(_result) {
		}
		void operator()(const T & x) const {
			result += x;
		}
	};
	template <class T>
	struct GetAbs {
		T operator()(const T & x) const {
			return std::abs(x);
		}
	};
	template <class T>
	struct GetMax {
		T operator()(const T & x, const T & y) const {
			return std::max(x, y);
		}
	};
	template <class T>
	struct GetMin {
		T operator()(const T & x, const T & y) const {
			return std::min(x, y);
		}
	};
	template <class T>
	struct GetAvg {
		T operator()(const T & x, const T & y) const {
			return (x + y) / 2;
		}
	};

public:
	/*!
	 * constructor
	 * @param dimensions the dimension of the vector
	 * @param value all components of this vector will be initialized with his value
	 * @return a new VecN
	 * @note the call VecNf(3,0); does not work, because 0 can be interpreted as nullptr pointer so the call gets
	 * ambiguous.
	 *          in this case remove the second parameter (no change as 0 is the default value) or add an explicit cast
	 * to value_t.
	 */
	_VecN(const size_t dimensions, value_t value = static_cast<value_t>(0)) : vec(dimensions, value) {
	}

	/*!
	 * constructor
	 * @param dimensions the dimension of the vector
	 * @param values an array of initial values
	 * @return a new VecN
	 * @note causes a segmentation fault if the length of values is smaller than the dimension of this
	 */
	_VecN(const size_t dimensions, const value_t * const values) : vec(values, values + dimensions) {
	}

	/*!
	 * constructor
	 * @param first
	 * @param end
	 * @return a new VecN containing the elements [first, end)
	 */
	template <typename _InputIterator>
	_VecN(_InputIterator _first, _InputIterator _end)
			: vec(_first, _end) {
	}

	//! destructor
	~_VecN() = default;

	//! copy constructor
	_VecN(const _VecN &) = default;

	//! move constructor
	_VecN(_VecN &&) = default;

	//! assignment operator
	_VecN & operator=(const _VecN &) = default;

	//! move assignment operator
	_VecN & operator=(_VecN &&) = default;

	/*!
	 * @return a pointer to the underlying data of this vector
	 */
	value_t * data() {
		return vec.data();
	}

	/*!
	 * @return a pointer to the underlying data of this vector
	 */
	const value_t * data() const {
		return vec.data();
	}

	/*!
	 * @return the number of components of this vector
	 */
	std::size_t size() const {
		return vec.size();
	}

	/*!
	 * copies the values from other to this VecN
	 * @param other
	 */
	void set(const _VecN & other) {
		vec = other.vec;
	}

	/*!
	 * @param values the values to be set for the components of this VecN
	 * @note causes a segmentation fault if the length of values is smaller than the dimension of this
	 */
	void set(const value_t * const values) {
		vec.assign(values, values + size());
	}

	/*!
	 * sets all components of this to value
	 * @param value
	 */
	void setAll(const value_t & value) {
		vec.assign(size(), value);
	}

	/*!
	 * read/write access to the x-th component of this
	 * @throw std::out_of_range iff x outside [0 .. size-1]
	 * @note should not be used for iteration because of slow access, use data() instead
	 */
	value_t & operator[](size_t x) {
		return vec.at(x);
	}

	/*!
	 * read-only access to the x-th component of this
	 * @throw std::out_of_range iff x outside [0 .. size-1]
	 * @note should not be used for iteration because of slow access, use data() instead
	 */
	const value_t & operator[](size_t x) const {
		return vec.at(x);
	}

	/*!
	 * read-only access to underlying data
	 */
	vecN_t::const_iterator begin() const {
		return vec.cbegin();
	}

	/*!
	 * read/write access to underlying data
	 */
	vecN_t::iterator begin() {
		return vec.begin();
	}

	/*!
	 * read-only access to underlying data
	 */
	vecN_t::const_iterator end() const {
		return vec.cend();
	}

	/*!
	 * read/write access to underlying data
	 */
	vecN_t::iterator end() {
		return vec.end();
	}

	/*!
	 * vector = constant, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	_VecN & operator=(const value_t & constant) {
		set(constant);
		return *this;
	}

	/*!
	 * vector += vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	_VecN & operator+=(const _VecN & _other) {
		checkSize(_other);
		std::transform(vec.cbegin(), vec.cend(), _other.vec.cbegin(), vec.begin(), std::plus<value_t>());
		return *this;
	}

	/*!
	 * vector += constant, component wise
	 */
	_VecN & operator+=(const value_t & constant) {
		std::transform(vec.begin(), vec.end(), vec.begin(),
					   [constant](const value_t & value) { return value + constant; });
		return *this;
	}

	/*!
	 * vector -= vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	_VecN & operator-=(const _VecN & _other) {
		checkSize(_other);
		std::transform(vec.cbegin(), vec.cend(), _other.vec.cbegin(), vec.begin(), std::minus<value_t>());
		return *this;
	}

	/*!
	 * vector -= constant, component wise
	 */
	_VecN & operator-=(const value_t & constant) {
		std::transform(vec.begin(), vec.end(), vec.begin(),
					   [constant](const value_t & value) { return value - constant; });
		return *this;
	}

	/*!
	 * vector *= vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	_VecN & operator*=(const _VecN & _other) {
		checkSize(_other);
		std::transform(vec.cbegin(), vec.cend(), _other.vec.cbegin(), vec.begin(), std::multiplies<value_t>());
		return *this;
	}

	/*!
	 * vector *= constant, component wise
	 */
	_VecN & operator*=(const float_t & constant) {
		std::transform(vec.begin(), vec.end(), vec.begin(),
					   [constant](const value_t & value) { return value * constant; });
		return *this;
	}

	/*!
	 * vector /= vector, component wise
	 */
	_VecN & operator/=(const _VecN & _other) {
		checkSize(_other);
		std::transform(vec.cbegin(), vec.cend(), _other.vec.cbegin(), vec.begin(), std::divides<value_t>());
		return *this;
	}

	/*!
	 * vector /= constant, component wise
	 */
	_VecN & operator/=(const float_t & constant) {
		std::transform(vec.begin(), vec.end(), vec.begin(),
					   [constant](const value_t & value) { return value / constant; });
		return *this;
	}

	/*!
	 * vector * vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 * @note this is not a dot product
	 */
	const _VecN operator*(const _VecN & _other) const {
		_VecN ret(*this);
		ret *= _other;
		return ret;
	}

	/*!
	 * vector * constant, component wise
	 */
	const _VecN operator*(const float_t & constant) const {
		_VecN ret(*this);
		ret *= constant;
		return ret;
	}

	/*!
	 * vector / vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	const _VecN operator/(const _VecN & _other) const {
		_VecN ret(*this);
		ret /= _other;
		return ret;
	}

	/*!
	 * vector / constant, component wise
	 */
	const _VecN operator/(const float_t & constant) const {
		_VecN ret(*this);
		ret /= constant;
		return ret;
	}

	/*!
	 * vector + vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	const _VecN operator+(const _VecN & _other) const {
		_VecN ret(*this);
		ret += _other;
		return ret;
	}

	/*!
	 * vector + constant, component wise
	 */
	const _VecN operator+(const value_t & constant) const {
		_VecN ret(*this);
		ret += constant;
		return ret;
	}

	/*!
	 * vector - vector, component wise
	 * @throw std::range_error iff other.size != this.size
	 */
	const _VecN operator-(const _VecN & _other) const {
		_VecN ret(*this);
		ret -= _other;
		return ret;
	}

	/*!
	 * vector - constant, component wise
	 */
	const _VecN operator-(const value_t & constant) const {
		_VecN ret(*this);
		ret -= constant;
		return ret;
	}

	/*!
	 * - vector, component wise
	 */
	const _VecN operator-() const {
		_VecN ret(size());
		std::transform(vec.cbegin(), vec.cend(), ret.vec.begin(), std::negate<value_t>());
		return ret;
	}

	/*!
	 * @return a vector containing componentwise the absulute values of this
	 */
	_VecN getAbs() const {
		return _VecN(*this).abs();
	}

	/*!
	 * modifies this by taking componentwise the absolute value
	 * @return this
	 */
	_VecN & abs() {
		std::transform(vec.cbegin(), vec.cend(), vec.begin(), GetAbs<value_t>());
		return *this;
	}

	/*!
	 * @return the maximum over the components of this vector
	 * @note this is not the maximum norm
	 */
	value_t max() const {
		return *std::max_element(vec.cbegin(), vec.cend());
	}

	/*!
	 * @return the minimum over the components of this vector
	 */
	value_t min() const {
		return *std::min_element(vec.cbegin(), vec.cend());
	}

	/*!
	 * @return the average over the components of this vector
	 */
	float_t avg() const {
		value_t sum = 0;
		std::for_each(vec.cbegin(), vec.cend(), Sum<value_t>(sum));
		return static_cast<float_t>(sum) / static_cast<float_t>(size());
	}

	/*! constant for the MAXIMUM or INFINITY or L-INF norm */
	static const uint32_t MAXIMUM_NORM = 0;
	/*! constant for the MANHATTAN or L-1 norm */
	static const uint32_t MANHATTAN_NORM = 1;
	/*! constant for the EUCLIDEAN or L-2 norm */
	static const uint32_t EUCLIDEAN_NORM = 2;

	float_t length(uint32_t norm) const {
		value_t ret = static_cast<value_t>(0);
		switch (norm) {
			case MAXIMUM_NORM: // 0
				for (const auto & it : vec) {
					ret = std::max(std::abs(it), ret);
				}
				return ret;
			case MANHATTAN_NORM: // 1
				for (const auto & it : vec) {
					ret += std::abs(it);
				}
				return ret;
			case EUCLIDEAN_NORM: // 2
				for (const auto & it : vec) {
					ret += it * it;
				}
				ret = std::sqrt(ret);
				return ret;
			default: // >= 3
				if (norm % 2 == 0) { // exponent is even --> std::abs on base IS NOT necessary
					for (const auto & it : vec) {
						ret += std::pow(it, static_cast<float_t>(norm));
					}
					ret = std::pow(ret, static_cast<float_t>(1) / static_cast<float_t>(norm));
				} else { // exponent is odd --> std::abs on base IS necessary (this works for all cases except maximum
						 // norm)
					for (const auto & it : vec) {
						ret += std::pow(std::abs(it), static_cast<float_t>(norm));
					}
					ret = std::pow(ret, static_cast<float_t>(1) / static_cast<float_t>(norm));
				}
				return ret;
		}
	}

	/*!
	 * @throw std::range_error iff vecA.size != vecB.size
	 */
	static _VecN pairwiseMax(const _VecN & vecA, const _VecN & vecB) {
		vecA.checkSize(vecB);
		_VecN ret(vecA.size());
		std::transform(vecA.vec.cbegin(), vecA.vec.cend(), vecB.vec.cbegin(), ret.vec.begin(), GetMax<value_t>());
		return ret;
	}

	/*!
	 * @throw std::range_error iff vecA.size != vecB.size
	 */
	static _VecN pairwiseMin(const _VecN & vecA, const _VecN & vecB) {
		vecA.checkSize(vecB);
		_VecN ret(vecA.size());
		std::transform(vecA.vec.cbegin(), vecA.vec.cend(), vecB.vec.cbegin(), ret.vec.begin(), GetMin<value_t>());
		return ret;
	}

	/*!
	 * @throw std::range_error iff vecA.size != vecB.size
	 */
	static _VecN pairwiseAvg(const _VecN & vecA, const _VecN & vecB) {
		vecA.checkSize(vecB);
		_VecN ret(vecA.size());
		std::transform(vecA.vec.cbegin(), vecA.vec.cend(), vecB.vec.cbegin(), ret.vec.begin(), GetAvg<value_t>());
		return ret;
	}

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const vecN_t & v) {
		out << static_cast<uint64_t>(v.size());
		for (const auto & value : v.vec) {
			out << ' ' << value;
		}
		return out;
	}
	friend std::istream & operator>>(std::istream & in, vecN_t & v) {
		uint64_t size;
		in >> size;
		v.vec.resize(size);
		for (auto & value : v.vec) {
			in >> value;
		}
		return in;
	}
	//@}
};

typedef _VecN<float, float> VecN;
typedef _VecN<float, float> VecNf;
typedef _VecN<double, double> VecNd;
typedef _VecN<int, float> VecNi;
}

#endif /* GEOMETRY_VECN_H */
