/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_RECT_H
#define GEOMETRY_RECT_H

#include "Definitions.h"
#include "Vec2.h"
#include <istream>
#include <ostream>

namespace Geometry {

/**
 * Two-dimensional rectangle.
 *
 @verbatim

 ^Y
 |
 |
 |
 o---> X

	--- width ----
	+------------+ |
	|            | |
	|            | height
	|            | |
	o------------+ |
 (x, y)

 @endverbatim
 */
template <typename T_>
class _Rect {
public:
	typedef T_ value_t;
	typedef _Vec2<value_t> vec2_t;

	/**
	 * @name Main
	 */
	//@{
	_Rect()
			: x(static_cast<value_t>(0)),
			  y(static_cast<value_t>(0)),
			  width(static_cast<value_t>(0)),
			  height(static_cast<value_t>(0)) {
	}
	_Rect(value_t posX, value_t posY, value_t w, value_t h) : x(posX), y(posY), width(w), height(h) {
	}
	_Rect(const vec2_t & pos, const vec2_t & size) : x(pos.x()), y(pos.y()), width(size.x()), height(size.y()) {
	}
	template <class other_t>
	explicit _Rect(const _Rect<other_t> & other)
			: x(static_cast<value_t>(other.getX())),
			  y(static_cast<value_t>(other.getY())),
			  width(static_cast<value_t>(other.getWidth())),
			  height(static_cast<value_t>(other.getHeight())) {
	}
	//@}

	/**
	 * @name Information
	 */
	//@{
	vec2_t getSize() const {
		return vec2_t(width, height);
	}
	value_t getX() const {
		return x;
	}
	value_t getMinX() const {
		return x;
	}
	value_t getMaxX() const {
		return x + width;
	}
	value_t getY() const {
		return y;
	}
	value_t getMinY() const {
		return y;
	}
	value_t getMaxY() const {
		return y + height;
	}
	value_t getWidth() const {
		return width;
	}
	value_t getHeight() const {
		return height;
	}
	value_t getArea() const {
		return width * height;
	}
	vec2_t getPosition() const {
		return vec2_t(x, y);
	}
	vec2_t getCenter() const {
		return vec2_t((getMinX() + getMaxX()) / static_cast<value_t>(2),
					  (getMinY() + getMaxY()) / static_cast<value_t>(2));
	}
	bool contains(value_t _x, value_t _y) const {
		return _x >= x && _x <= x + width && _y >= y && _y <= y + height;
	}
	bool contains(const _Rect<value_t> & rect) const {
		return contains(rect.getMinX(), rect.getMinY()) && contains(rect.getMaxX(), rect.getMaxY());
	}
	bool contains(const vec2_t & vec) const {
		return contains(vec.getX(), vec.getY());
	}
	bool isInvalid() const {
		return width < static_cast<value_t>(0) || height < static_cast<value_t>(0);
	}
	bool isValid() const {
		return width >= static_cast<value_t>(0) && height >= static_cast<value_t>(0);
	}
	bool intersects(const _Rect<value_t> & rect) const {
		return isValid() && rect.isValid() && rect.getMaxX() >= getMinX() && rect.getMinX() <= getMaxX()
				&& rect.getMaxY() >= getMinY() && rect.getMinY() <= getMaxY();
	}
	vec2_t getCorner(const rectCorner_t corner) const {
		auto nr = static_cast<std::size_t>(corner);
		return vec2_t((nr & 1) ? getMaxX() : getMinX(), (nr & 2) ? getMaxY() : getMinY());
	}

	//@}

	/**
	 * @name Read-Write Access
	 * @note Do not use these functions, if not really necessary.
	 */
	//@{
	value_t & _accessX() {
		return x;
	}
	value_t & _accessY() {
		return y;
	}
	value_t & _accessWidth() {
		return width;
	}
	value_t & _accessHeight() {
		return height;
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	void setPosition(value_t _x, value_t _y) {
		x = _x;
		y = _y;
	}
	void setPosition(const vec2_t & vec) {
		x = vec.getX();
		y = vec.getY();
	}
	void setX(value_t _x) {
		x = _x;
	}
	void setY(value_t _y) {
		y = _y;
	}
	void setWidth(value_t w) {
		width = w;
	}
	void setHeight(value_t h) {
		height = h;
	}
	void setSize(value_t w, value_t h) {
		width = w;
		height = h;
	}
	void setSize(const vec2_t & s) {
		width = s.x();
		height = s.y();
	}

	void invalidate() {
		width = static_cast<value_t>(-1);
		height = static_cast<value_t>(-1);
	}

	_Rect<value_t> & moveRel(value_t _x, value_t _y) {
		x += _x;
		y += _y;
		return *this;
	}
	_Rect<value_t> & moveRel(const vec2_t & vec) {
		x += vec.getX();
		y += vec.getY();
		return *this;
	}
	//! Move by given vector
	_Rect<value_t> & operator+=(const vec2_t & vec) {
		return moveRel(vec);
	}
	//! Move by given vector in negative direction
	_Rect<value_t> & operator-=(const vec2_t & vec) {
		return moveRel(-vec);
	}
	//! Change size around upper left corner
	_Rect<value_t> & changeSize(value_t dw, value_t dh) {
		width += dw;
		height += dh;
		return *this;
	}
	//! Change size around upper left corner
	_Rect<value_t> & changeSize(const vec2_t & vec) {
		return changeSize(vec.getWidth(), vec.getHeight());
	}
	//! Change size around center
	_Rect<value_t> & changeSizeCentered(value_t dw, value_t dh) {
		changeSize(dw, dh);
		moveRel(-dw / static_cast<value_t>(2), -dh / static_cast<value_t>(2));
		return *this;
	}
	//! Change size around center
	_Rect<value_t> & changeSizeCentered(const vec2_t & vec) {
		return changeSizeCentered(vec.getWidth(), vec.getHeight());
	}
	_Rect<value_t> & include(const vec2_t & vec) {
		return include(vec.getX(), vec.getY());
	}
	_Rect<value_t> & include(value_t px, value_t py) {
		if (isInvalid()) {
			x = px;
			y = py;
			width = static_cast<value_t>(0);
			height = static_cast<value_t>(0);
			return *this;
		}
		if (px > getMaxX()) {
			width = px - x;
		} else if (px < getMinX()) {
			width += x - px;
			x = px;
		}
		if (py > getMaxY()) {
			height = py - y;
		} else if (py < y) {
			height += y - py;
			y = py;
		}
		return *this;
	}
	_Rect<value_t> & include(const _Rect<value_t> & rect) {
		if (rect.isInvalid()) {
			return *this;
		} else if (isInvalid()) {
			*this = rect;
		} else {
			include(rect.getMinX(), rect.getMinY());
			include(rect.getMaxX(), rect.getMaxY());
		}
		return *this;
	}

	_Rect<value_t> & clipBy(const _Rect<value_t> & rect) {
		if (!intersects(rect)) {
			invalidate();
			return *this;
		}
		if (getMinX() < rect.getMinX()) {
			width -= rect.getMinX() - getMinX();
			x = rect.getMinX();
		}
		if (getMaxX() > rect.getMaxX()) {
			width -= getMaxX() - rect.getMaxX();
		}
		if (getMinY() < rect.getMinY()) {
			height -= rect.getMinY() - getMinY();
			y = rect.getMinY();
		}
		if (getMaxY() > rect.getMaxY()) {
			height -= getMaxY() - rect.getMaxY();
		}
		return *this;
	}
	//@}

	/**
	 * @name Creation
	 */
	//@{
	//! Create Rect by moving this by given vector
	const _Rect<value_t> operator+(const vec2_t & vec) const {
		return _Rect<value_t>(x + vec.getX(), y + vec.getY(), width, height);
	}
	//! Create Rect by moving this by given vector in negative direction
	const _Rect<value_t> operator-(const vec2_t & vec) const {
		return _Rect<value_t>(x - vec.getX(), y - vec.getY(), width, height);
	}
	//@}

	/**
	 * @name Comparators
	 */
	//@{
	bool operator==(const _Rect<value_t> & rect) const {
		return rect.x == x && rect.y == y && rect.width == width && rect.height == height;
	}
	bool operator!=(const _Rect<value_t> & rect) const {
		return rect.x != x || rect.y != y || rect.width != width || rect.height != height;
	}
	//@}

private:
	value_t x;
	value_t y;
	value_t width;
	value_t height;

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const _Rect<value_t> & rect) {
		return out << rect.x << ' ' << rect.y << ' ' << rect.width << ' ' << rect.height;
	}
	friend std::istream & operator>>(std::istream & in, _Rect<value_t> & rect) {
		return in >> rect.x >> rect.y >> rect.width >> rect.height;
	}
	//@}
};

typedef _Rect<float> Rect;
typedef _Rect<float> Rect_f;
typedef _Rect<int> Rect_i;
typedef _Rect<double> Rect_d;
}

#endif /* GEOMETRY_RECT_H */
