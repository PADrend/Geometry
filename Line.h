/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_LINE_H
#define GEOMETRY_LINE_H

#include "Vec2.h"
#include "Vec3.h"
#include <algorithm>
#include <limits>
#include <utility>

namespace Geometry {

//! The common abstract base class for Lines, Rays and Segments.
template <typename vec_t, template <class> class _impl_t>
class _LineBase {
public:
	typedef _impl_t<vec_t> impl_t;
	typedef typename vec_t::value_t value_t; // use the same data type as the used vectors
private:
	vec_t origin;
	vec_t direction;
	value_t minParam;
	value_t maxParam;

	//! @name Construction
	//@{
protected:
	_LineBase(vec_t _origin, vec_t _direction, value_t _minParam, value_t _maxParam)
			: origin(std::move(_origin)), direction(std::move(_direction)), minParam(_minParam), maxParam(_maxParam) {
	}
	//@}

	//! @name Information
	//@{
public:
	value_t getMinParam() const {
		return minParam;
	}
	value_t getMaxParam() const {
		return maxParam;
	}
	const vec_t & getDirection() const {
		return direction;
	}
	const vec_t & getOrigin() const {
		return origin;
	}

	/**
	 * Return an arbitrary point on the line.
	 *
	 * @param t Parameter specifying the point. If @a t is not in the interval [@a minParam, @a maxParam],
	 *			the smallest/greatest valid value is used.
	 * @return Point p fulfilling p = @a origin + @a direction * @a t.
	 */
	vec_t getPoint(value_t t) const {
		t = std::max(minParam, std::min(t, maxParam));
		return origin + direction * t;
	}
	/*! Return the point on the line that has minimal distance to the given point.
		\note The direction vector is required to have unit length. */
	vec_t getClosestPoint(const vec_t & p) const {
		const value_t t = direction.dot(p - origin);
		return getPoint(t);
	}
	/*! Return the distance of the line to the given point.
		\note The direction vector is required to have unit length.	*/
	value_t distance(const vec_t & p) const {
		return (getClosestPoint(p) - p).length();
	}
	bool operator==(const impl_t & other) const {
		return origin == other.origin && direction == other.direction && minParam == other.minParam
				&& maxParam == other.maxParam;
	}
	//@}

	//! @name Modification
	//@{
protected: // \note these methods are proteced as they should not be available in all subclasses.
	/**
	 * Specify a new direction for the line.
	 * @note The direction is required to have unit length for any distance calculation.
	 * @param newDirection Normalized direction vector
	 */
	void _setDirection(const vec_t & newDirection) {
		direction = newDirection;
	}
	/**
	 * Specify a new base vector for the line.
	 *
	 * @param newOrigin Point vector
	 */
	void _setOrigin(const vec_t & newOrigin) {
		origin = newOrigin;
	}

	/**
	 * Normalize the direction vector.
	 * This should be done before any distance calculation!
	 */
	void _normalize() {
		direction.normalize();
	}
	//@}
};

// -----------------------------------------------------------------------------------------------------------
template <typename vec_t>
class _Ray; // forward declaration

/**
 * Line defined by an @a origin and a @a direction.
 * Points p on the line fulfill the equation p = @a origin + @a direction * t for arbitrary values of t.
 */
template <typename vec_t>
class _Line : public _LineBase<vec_t, _Line> {
	typedef _LineBase<vec_t, Geometry::_Line> base_t;
	typedef _Line<vec_t> line_t;
	typedef _Ray<vec_t> ray_t;
	typedef typename vec_t::value_t value_t;

public:
	_Line(const vec_t & _origin, const vec_t & _direction)
			: base_t(_origin, _direction, -std::numeric_limits<value_t>::infinity(),
					 std::numeric_limits<value_t>::infinity()) {
	}
	_Line()
			: base_t(vec_t(), vec_t(), -std::numeric_limits<value_t>::infinity(),
					 std::numeric_limits<value_t>::infinity()) {
	}

	/**
	 * Calculate a pair of points (= values for points) on this line and the given @a lineB that is closest to each
	 *other.
	 *
	 * @param lineB The second line
	 * @return pair(parameterA,parameterB) Return value that can be used to calculate the point on the lines
	 * @note The function @a _Line<_T>::getPoint can be used with the returned parameters as argument to calculate the
	 *points.
	 * @note The direction vectors is required to have unit length.
	 * @author Benjamin Eikel
	 * @date 2010-10-14
	 */
	std::pair<value_t, value_t> getClosestValues(const line_t & lineB) const {
		const value_t zero = static_cast<value_t>(0);
		const value_t one = static_cast<value_t>(1);
		const line_t & lineA = *this;

		/*
		 * We have to minimize a function of the form f(x, y) = a x² + 2 b x y + c y² + 2 d x + 2 e y + f.
		 * The gradient is ∇f(x, y) = (δf/δx, δf/δy) = (2 a x + 2 b y + 2 d, 2 c y + 2 b x + 2 e).
		 * The Hessian matrix is H(f) = ( (δ²f/(δx δx), δ²f/(δx δy)), (δ²f/(δy δx), δ²f/(δy δy)) ) = ( (2 a, 2 b), (2 b,
		 * 2 c) ).
		 * The determinant of H(f) is 2 a * 2 c - 2 b * 2 b = 4 (a c - b²).
		 * The roots of the gradient are x = - (b e - c d) / (a c - b²) and y = - (b d - a e) / (a c - b²).
		 */
		// a = lineA.getDirection() * lineA.getDirection() = 1 (because directions of lines have unit length)
		const value_t b = -lineA.getDirection().dot(lineB.getDirection());
		// c = lineB.getDirection() * lineB.getDirection() = 1 (because directions of lines have unit length)
		const value_t det = one - b * b; // ignore the 4 here, because we are interested in the sign only

		const vec_t diff = lineA.getOrigin() - lineB.getOrigin();
		const value_t d = lineA.getDirection().dot(diff);

		if (det >= std::numeric_limits<value_t>::epsilon()) {
			// The lines are not parallel.
			const value_t e = -lineB.getDirection().dot(diff);
			const value_t invDet = one / det;
			return std::make_pair((b * e - d) * invDet, // parameterA: drop c here because it is 1
								  (b * d - e) * invDet); // parameterB: drop a here because it is 1
		} else {
			// The lines are parallel. Pick a pair arbitrary matching closest pair of points.
			return std::make_pair(-d, zero);
		}
	}
	/**
	 * Calculate a pair of points (= values for points) on this line and given @a ray that is closest to each other.
	 *
	 * @param ray The ray
	 * @return pair(parameterA,parameterB) Return value that can be used to calculate the point on the line and the ray
	 * @note The function @a _Line<_T>::getPoint and @a _Ray<_T>::getPoint can be used with the returned parameters as
	 *argument to calculate the points.
	 * @note The direction vectors is required to have unit length.
	 * @author Benjamin Eikel
	 * @date 2010-10-14
	 */
	std::pair<value_t, value_t> getClosestValues(const ray_t & ray) const {
		const value_t zero = static_cast<value_t>(0);
		const value_t one = static_cast<value_t>(1);
		const line_t & line = *this;
		/*
		 * We have to minimize a function of the form f(x, y) = a x² + 2 b x y + c y² + 2 d x + 2 e y + f.
		 * The gradient is ∇f(x, y) = (δf/δx, δf/δy) = (2 a x + 2 b y + 2 d, 2 c y + 2 b x + 2 e).
		 * The Hessian matrix is H(f) = ( (δ²f/(δx δx), δ²f/(δx δy)), (δ²f/(δy δx), δ²f/(δy δy)) ) = ( (2 a, 2 b), (2 b,
		 * 2 c) ).
		 * The determinant of H(f) is 2 a * 2 c - 2 b * 2 b = 4 (a c - b²).
		 * The roots of the gradient are x = - (b e - c d) / (a c - b²) and y = - (b d - a e) / (a c - b²).
		 */
		// a = line.getDirection() * line.getDirection() = 1 (because directions of lines have unit length)
		const value_t b = -line.getDirection().dot(ray.getDirection());
		// c = ray.getDirection() * ray.getDirection() = 1 (because directions of lines have unit length)
		const value_t det = one - b * b; // ignore the 4 here, because we are interested in the sign only

		const vec_t diff = line.getOrigin() - ray.getOrigin();
		const value_t d = line.getDirection().dot(diff);

		if (det >= std::numeric_limits<value_t>::epsilon()) {
			// The line and the ray are not parallel.
			const value_t e = -ray.getDirection().dot(diff);
			// Check if the closest pair lies in the negative direction of the ray.
			const value_t rayParam = b * d - e;
			if (rayParam >= zero) {
				const value_t invDet = one / det;
				return std::make_pair((b * e - d) * invDet, // parameterA: drop c here because it is 1
									  (b * d - e) * invDet); // parameterB: drop a here because it is 1
			} else {
				return std::make_pair(-d, zero);
			}
		} else {
			// The line and the ray are parallel. Pick a pair arbitrary matching closest pair of points.
			return std::make_pair(-d, zero);
		}
	}

	vec_t getClosestPointToRay(const ray_t & ray) const {
		return base_t::getPoint(getClosestValues(ray).first);
	}

	void normalize() {
		base_t::_normalize();
	}
	void setDirection(const vec_t & newDirection) {
		base_t::_setDirection(newDirection);
	}
	void setOrigin(const vec_t & newOrigin) {
		base_t::_setOrigin(newOrigin);
	}
};

typedef _Line<Vec2f> Line2;
typedef _Line<Vec2f> Line2f;
typedef _Line<Vec2d> Line2d;
typedef _Line<Vec3f> Line3;
typedef _Line<Vec3f> Line3f;
typedef _Line<Vec3d> Line3d;

// --------------------------------------------------------------
//
/**
 * Ray defined by an @a origin and a @a direction.
 * Points p on the ray fulfill the equation p = @a origin + @a direction * t for t >= 0.
 */
template <typename vec_t>
class _Ray : public _LineBase<vec_t, _Ray> {
	typedef _LineBase<vec_t, Geometry::_Ray> base_t;
	typedef typename vec_t::value_t value_t;

public:
	_Ray(const vec_t & _origin, const vec_t & _direction)
			: base_t(_origin, _direction, static_cast<value_t>(0), std::numeric_limits<value_t>::infinity()) {
	}
	_Ray() : base_t(vec_t(), vec_t(), static_cast<value_t>(0), std::numeric_limits<value_t>::infinity()) {
	}
	//! \see _Line::getClosestValues(Ray)
	std::pair<value_t, value_t> getClosestValues(const _Line<vec_t> & line) const {
		std::pair<value_t, value_t> result = line.getClosestValues(*this);
		return std::make_pair(result.second, result.first);
	}
	void normalize() {
		base_t::_normalize();
	}
	void setDirection(const vec_t & newDirection) {
		base_t::_setDirection(newDirection);
	}
	void setOrigin(const vec_t & newOrigin) {
		base_t::_setOrigin(newOrigin);
	}
};

typedef _Ray<Vec2f> Ray2;
typedef _Ray<Vec2f> Ray2f;
typedef _Ray<Vec2d> Ray2d;
typedef _Ray<Vec3f> Ray3;
typedef _Ray<Vec3f> Ray3f;
typedef _Ray<Vec3d> Ray3d;

// --------------------------------------------------------------

/**
 * Segment defined by two points @a fromPoint and @a toPoint.
 * Points p on the segment fulfill the equation p = @a fromPoint + @a (toPoint-fromPoint).normalize * t for values of t
 * in the interval [0, (toPoint-fromPoint).length].
 * @note the direction vector is automatically normalized in the constructor.
 */
template <typename vec_t>
class _Segment : public _LineBase<vec_t, _Segment> {
	typedef _LineBase<vec_t, Geometry::_Segment> base_t;
	typedef _Segment<vec_t> segment_t;
	typedef typename vec_t::value_t value_t;

public:
	_Segment(const vec_t & fromPoint, const vec_t & toPoint)
			: base_t(fromPoint, (toPoint - fromPoint), static_cast<value_t>(0),
					 static_cast<value_t>((toPoint - fromPoint).length())) {
		if (base_t::getMaxParam() > 0.0) {
			base_t::_setDirection(base_t::getDirection() / base_t::getMaxParam());
		}
	}
	_Segment() : base_t(vec_t(), vec_t(), static_cast<value_t>(0), static_cast<value_t>(0)) {
	}
	value_t length() const {
		return base_t::getMaxParam();
	}

	const vec_t & getFirstPoint() const {
		return base_t::getOrigin();
	}
	vec_t getSecondPoint() const {
		return base_t::getPoint(base_t::getMaxParam());
	}

	void setFirstPoint(const vec_t & p) {
		(*this) = segment_t(p, getSecondPoint());
	}
	void setSecondPoint(const vec_t & p) {
		(*this) = segment_t(getFirstPoint(), p);
	}
};

typedef _Segment<Vec2f> Segment2;
typedef _Segment<Vec2f> Segment2f;
typedef _Segment<Vec2d> Segment2d;
typedef _Segment<Vec3f> Segment3;
typedef _Segment<Vec3f> Segment3f;
typedef _Segment<Vec3d> Segment3d;
}
#endif /* GEOMETRY_LINE_H */
