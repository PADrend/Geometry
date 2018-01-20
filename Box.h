/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_BOX_H
#define GEOMETRY_BOX_H

#include "Definitions.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace Geometry {

/**
 * Three-dimensional axis-aligned box.
 * [Box]
 */
template <typename value_t>
class _Box {
public:
	using vec3_t = _Vec3<value_t>;

private:
	vec3_t min;
	vec3_t max;

public:
	/**
	 * @name Main
	 */
	//@{
	_Box() : min(-0.0, -0.0, -0.0), max(0.0, 0.0, 0.0) {
	}
	_Box(value_t minx, value_t maxx, value_t miny, value_t maxy, value_t minz, value_t maxz)
			: min(minx, miny, minz), max(maxx, maxy, maxz) {
	}
	_Box(const vec3_t & center, value_t size)
			: min(center.getX() - size * 0.5, center.getY() - size * 0.5, center.getZ() - size * 0.5),
			  max(center.getX() + size * 0.5, center.getY() + size * 0.5, center.getZ() + size * 0.5) {
	}
	_Box(const vec3_t & center, value_t dimX, value_t dimY, value_t dimZ)
			: min(center.getX() - dimX * 0.5, center.getY() - dimY * 0.5, center.getZ() - dimZ * 0.5),
			  max(center.getX() + dimX * 0.5, center.getY() + dimY * 0.5, center.getZ() + dimZ * 0.5) {
	}
	_Box(const vec3_t & cornerA, const vec3_t & cornerB)
			: min(vec3_t::pairwiseMin(cornerA, cornerB)), max(vec3_t::pairwiseMax(cornerA, cornerB)) {
	}

private:
	inline void assertCorrectDimension(dimension_t dim) const;
	//@}

	/**
	 * @name Information
	 */
	//@{
public:
	value_t getMaxX() const {
		return max.getX();
	}
	value_t getMaxY() const {
		return max.getY();
	}
	value_t getMaxZ() const {
		return max.getZ();
	}
	inline value_t getMax(dimension_t dim) const;
	const vec3_t & getMax() const {
		return max;
	}
	value_t getMinX() const {
		return min.getX();
	}
	value_t getMinY() const {
		return min.getY();
	}
	value_t getMinZ() const {
		return min.getZ();
	}
	inline value_t getMin(dimension_t dim) const;
	const vec3_t & getMin() const {
		return min;
	}
	inline value_t getExtentMax() const;
	inline value_t getExtentMin() const;
	value_t getExtentX() const {
		return max.getX() - min.getX();
	}
	value_t getExtentY() const {
		return max.getY() - min.getY();
	}
	value_t getExtentZ() const {
		return max.getZ() - min.getZ();
	}
	inline value_t getExtent(dimension_t dim) const;

	/**
	 * @return length of the space diagonal
	 */
	inline value_t getDiameter() const;

	/**
	 * @return squared length of the space diagonal
	 */
	inline value_t getDiameterSquared() const;

	inline value_t getVolume() const;
	inline value_t getSurfaceArea() const;

	inline vec3_t getCorner(const corner_t corner) const;
	/**
	 * Retrieve the corner index which does not share any side with the given corner index.
	 *
	 * @param corner Index of a box corner.
	 * @return Index of the corner that is on the other end of the box's diagonal starting at the given corner.
	 */
	static corner_t getOppositeCorner(const corner_t corner) {
		return static_cast<corner_t>(static_cast<std::size_t>(corner) ^ 7);
	}
	/*
	 * calculates the corner of this box which is in the same octant as the given vector
	 * it is not checked if the vector is contained in this box,
	 * calculation is done by assuming a coordinate system in the center of ttis box.
	 */
	inline corner_t getOctant(const vec3_t & v) const;

	inline bool contains(value_t x, value_t y, value_t z) const;
	inline bool contains(const vec3_t & p) const;
	inline bool contains(const _Box & b) const;

	/**
	 * @param p Point to be used for calculations
	 * @return the distance between this and p
	 *
	 * @note distance is defined as the minimum distance p has
	 * to be moved such that this.contains(p) returns true
	 */
	inline value_t getDistance(const vec3_t & p) const;
	/**
	 * same as getDistance, but faster because of missing sqrt() call
	 */
	inline value_t getDistanceSquared(const vec3_t & p) const;
	
	inline vec3_t getClosestPoint(const vec3_t & p) const;
	
	inline bool isValid() const;
	bool isInvalid() const {
		return !isValid();
	}

	vec3_t getCenter() const {
		return (min + max) * 0.5;
	}
	value_t getBoundingSphereRadius() const {
		return getDiameter() * 0.5;
	}
	//@}

	/**
	 * @name Modification
	 */
	//@{
	inline void invalidate();
	void setMaxX(value_t x) {
		max.setX(x);
	}
	void setMaxY(value_t y) {
		max.setY(y);
	}
	void setMaxZ(value_t z) {
		max.setZ(z);
	}
	inline void setMax(dimension_t dim, value_t value);
	void setMax(const vec3_t & newMax) {
		max = newMax;
	}
	void setMinX(value_t x) {
		min.setX(x);
	}
	void setMinY(value_t y) {
		min.setY(y);
	}
	void setMinZ(value_t z) {
		min.setZ(z);
	}
	inline void setMin(dimension_t dim, value_t value);
	void setMin(const vec3_t & newMin) {
		min = newMin;
	}
	inline void include(const _Box & b);
	inline void include(const value_t x, const value_t y, const value_t z);
	inline void set(value_t minX, value_t maxX, value_t minY, value_t maxY, value_t minZ, value_t maxZ);
	inline void set(value_t x, value_t y, value_t z);
	inline void include(const vec3_t & p);
	inline void setCenter(const vec3_t & v);
	inline void resizeAbs(value_t size);
	inline void resizeRel(value_t relSize);
	inline void resizeAbs(value_t sizeX, value_t sizeY, value_t sizeZ);
	inline void resizeRel(value_t relSizeX, value_t relSizeY, value_t relSizeZ);
	void translate(const vec3_t & v) {
		min += v;
		max += v;
	}
	inline void setExtent(value_t ex);
	inline void setExtentX(value_t ex);
	inline void setExtentY(value_t ey);
	inline void setExtentZ(value_t ez);
	//@}

	/**
	 * @name Comparators
	 */
	//@{
	bool operator==(const _Box & b) const {
		return min == b.min && max == b.max;
	}
	bool operator!=(const _Box & b) const {
		return min != b.min || max != b.max;
	}
	//@}

	//! @name Serialization
	//@{
public:
	friend std::ostream & operator<<(std::ostream & out, const _Box & box) {
		return out << box.min << ' ' << box.max;
	}
	friend std::istream & operator>>(std::istream & in, _Box & box) {
		return in >> box.min >> box.max;
	}
	//@}
};
using Box = _Box<float>;

// --------------------------------------------------------------------------------------------------------------------------
// -----------------------------------
// ---- Information
template <typename value_t>
inline void _Box<value_t>::assertCorrectDimension(dimension_t dim) const {
	if ((dimension_t::X != dim) && (dimension_t::Y != dim) && (dimension_t::Z != dim)) {
		throw std::invalid_argument("Parameter \"dim\" has to be dimension_t::X/Y/Z");
	}
}

template <typename value_t>
inline value_t _Box<value_t>::getMax(dimension_t dim) const {
	assertCorrectDimension(dim);
	return max[static_cast<uint8_t>(dim)];
}

template <typename value_t>
inline value_t _Box<value_t>::getMin(dimension_t dim) const {
	assertCorrectDimension(dim);
	return min[static_cast<uint8_t>(dim)];
}

template <typename value_t>
inline value_t _Box<value_t>::getExtent(dimension_t dim) const {
	switch (dim) {
		case dimension_t::X:
			return getExtentX();
		case dimension_t::Y:
			return getExtentY();
		case dimension_t::Z:
			return getExtentZ();
		default:
			assertCorrectDimension(dim);
	}
	return 0.0; // never reached.
}

template <typename value_t>
inline value_t _Box<value_t>::getExtentMax() const {
	return std::max(std::max(getExtentX(), getExtentY()), getExtentZ());
}

template <typename value_t>
inline value_t _Box<value_t>::getExtentMin() const {
	return std::min(std::min(getExtentX(), getExtentY()), getExtentZ());
}

template <typename value_t>
inline value_t _Box<value_t>::getVolume() const {
	return getExtentX() * getExtentY() * getExtentZ();
}

template <typename value_t>
inline value_t _Box<value_t>::getDiameter() const {
	return std::sqrt(getDiameterSquared());
}

template <typename value_t>
inline value_t _Box<value_t>::getDiameterSquared() const {
	return getExtentX() * getExtentX() + getExtentY() * getExtentY() + getExtentZ() * getExtentZ();
}

template <typename value_t>
inline value_t _Box<value_t>::getSurfaceArea() const {
	return 2.0 * getExtentX() * getExtentY() + 2.0 * getExtentY() * getExtentZ() + 2.0 * getExtentX() * getExtentZ();
}

template <typename value_t>
inline _Vec3<value_t> _Box<value_t>::getCorner(const corner_t corner) const {
	const std::size_t nr = static_cast<std::size_t>(corner);
	return vec3_t((nr & 1) ? max.getX() : min.getX(), (nr & 2) ? max.getY() : min.getY(),
				  (nr & 4) ? max.getZ() : min.getZ());
}

template <typename value_t>
inline corner_t _Box<value_t>::getOctant(const vec3_t & v) const {
	unsigned index = 0;
	const vec3_t center = getCenter();
	if (v.getX() > center.getX()) {
		index |= (1 << 0);
	}
	if (v.getY() > center.getY()) {
		index |= (1 << 1);
	}
	if (v.getZ() > center.getZ()) {
		index |= (1 << 2);
	}
	return static_cast<corner_t>(index);
}

template <typename value_t>
inline bool _Box<value_t>::isValid() const {
	return max.getX() - min.getX() >= -std::numeric_limits<value_t>::epsilon()
			&& max.getY() - min.getY() >= -std::numeric_limits<value_t>::epsilon()
			&& max.getZ() - min.getZ() >= -std::numeric_limits<value_t>::epsilon();
}

template <typename value_t>
inline bool _Box<value_t>::contains(value_t x, value_t y, value_t z) const {
	return x >= min.getX() && x <= max.getX() && y >= min.getY() && y <= max.getY() && z >= min.getZ()
			&& z <= max.getZ();
}

template <typename value_t>
inline bool _Box<value_t>::contains(const vec3_t & p) const {
	return contains(p.getX(), p.getY(), p.getZ());
}

template <typename value_t>
inline bool _Box<value_t>::contains(const _Box & b) const {
	return min.getX() <= b.min.getX() && max.getX() >= b.max.getX() && min.getY() <= b.min.getY()
			&& max.getY() >= b.max.getY() && min.getZ() <= b.min.getZ() && max.getZ() >= b.max.getZ();
}

template <typename value_t>
inline value_t _Box<value_t>::getDistance(const vec3_t & p) const {
	return sqrt(getDistanceSquared(p));
}

template <typename value_t>
inline value_t _Box<value_t>::getDistanceSquared(const vec3_t & p) const {
	value_t x = 0;
	if (p.x() < min.getX()) {
		x = min.getX() - p.x();
	} else if (p.x() > max.getX()) {
		x = max.getX() - p.x();
	}
	value_t y = 0;
	if (p.y() < min.getY()) {
		y = min.getY() - p.y();
	} else if (p.y() > max.getY()) {
		y = max.getY() - p.y();
	}
	value_t z = 0;
	if (p.z() < min.getZ()) {
		z = min.getZ() - p.z();
	} else if (p.z() > max.getZ()) {
		z = max.getZ() - p.z();
	}
	return x * x + y * y + z * z;
}

template <typename value_t>
inline _Vec3<value_t> _Box<value_t>::getClosestPoint(const vec3_t & p) const {
	vec3_t closest(p);
	if (p.x() < min.getX()) {
		closest.setX(min.getX());
	} else if (p.x() > max.getX()) {
		closest.setX(max.getX());
	}
	if (p.y() < min.getY()) {
		closest.setY(min.getY());
	} else if (p.y() > max.getY()) {
		closest.setY(max.getY());
	}
	if (p.z() < min.getZ()) {
		closest.setZ(min.getZ());
	} else if (p.z() > max.getZ()) {
		closest.setZ(max.getZ());
	}
	return closest;
}

// -----------------------------------
// ---- Modification
template <typename value_t>
inline void _Box<value_t>::invalidate() {
	min.setX(1.0);
	max.setX(0.0);
}

template <typename value_t>
inline void _Box<value_t>::setMax(dimension_t dim, value_t value) {
	assertCorrectDimension(dim);
	max[static_cast<uint8_t>(dim)] = value;
}

template <typename value_t>
inline void _Box<value_t>::setMin(dimension_t dim, value_t value) {
	assertCorrectDimension(dim);
	min[static_cast<uint8_t>(dim)] = value;
}

template <typename value_t>
inline void _Box<value_t>::include(const _Box & b) {
	if (b.isInvalid()) {
		return;
	}
	if (isInvalid()) {
		*this = b;
		return;
	}
	if (b.min.getX() < min.getX()) {
		min.setX(b.min.getX());
	}
	if (b.max.getX() > max.getX()) {
		max.setX(b.max.getX());
	}
	if (b.min.getY() < min.getY()) {
		min.setY(b.min.getY());
	}
	if (b.max.getY() > max.getY()) {
		max.setY(b.max.getY());
	}
	if (b.min.getZ() < min.getZ()) {
		min.setZ(b.min.getZ());
	}
	if (b.max.getZ() > max.getZ()) {
		max.setZ(b.max.getZ());
	}
}

template <typename value_t>
inline void _Box<value_t>::include(const value_t x, const value_t y, const value_t z) {
	if (isInvalid()) {
		set(x, y, z);
		return;
	}
	if (x < min.getX()) {
		min.setX(x);
	} else if (x > max.getX()) {
		max.setX(x);
	}
	if (y < min.getY()) {
		min.setY(y);
	} else if (y > max.getY()) {
		max.setY(y);
	}
	if (z < min.getZ()) {
		min.setZ(z);
	} else if (z > max.getZ()) {
		max.setZ(z);
	}
}

template <typename value_t>
inline void _Box<value_t>::include(const vec3_t & p) {
	include(p.getX(), p.getY(), p.getZ());
}

template <typename value_t>
inline void _Box<value_t>::set(value_t minX, value_t maxX, value_t minY, value_t maxY, value_t minZ, value_t maxZ) {
	min.setValue(minX, minY, minZ);
	max.setValue(maxX, maxY, maxZ);
}

template <typename value_t>
inline void _Box<value_t>::set(value_t x, value_t y, value_t z) {
	min.setValue(x, y, z);
	max.setValue(x, y, z);
}

template <typename value_t>
inline void _Box<value_t>::setCenter(const vec3_t & v) {
	translate(v - getCenter());
}

template <typename value_t>
inline void _Box<value_t>::resizeAbs(value_t size) {
	resizeAbs(size, size, size);
}

template <typename value_t>
inline void _Box<value_t>::resizeRel(value_t relSize) {
	resizeRel(relSize, relSize, relSize);
}

template <typename value_t>
inline void _Box<value_t>::resizeAbs(value_t sizeX, value_t sizeY, value_t sizeZ) {
	const vec3_t resizeVec(sizeX, sizeY, sizeZ);
	min -= resizeVec;
	max += resizeVec;
}

template <typename value_t>
inline void _Box<value_t>::resizeRel(value_t relSizeX, value_t relSizeY, value_t relSizeZ) {
	const value_t halfChangeLengthX = (relSizeX - 1.0) * getExtentX() * 0.5;
	const value_t halfChangeLengthY = (relSizeY - 1.0) * getExtentY() * 0.5;
	const value_t halfChangeLengthZ = (relSizeZ - 1.0) * getExtentZ() * 0.5;

	const vec3_t resizeVec(halfChangeLengthX, halfChangeLengthY, halfChangeLengthZ);

	min -= resizeVec;
	max += resizeVec;
}

template <typename value_t>
inline void _Box<value_t>::setExtent(value_t ex) {
	setExtentX(ex);
	setExtentY(ex);
	setExtentZ(ex);
}

template <typename value_t>
inline void _Box<value_t>::setExtentX(value_t ex) {
	const value_t center = (min.getX() + max.getX()) * 0.5;
	const value_t halfEx = ex * 0.5;
	min.setX(center - halfEx);
	max.setX(center + halfEx);
}

template <typename value_t>
inline void _Box<value_t>::setExtentY(value_t ey) {
	const value_t center = (min.getY() + max.getY()) * 0.5;
	const value_t halfEx = ey * 0.5;
	min.setY(center - halfEx);
	max.setY(center + halfEx);
}

template <typename value_t>
inline void _Box<value_t>::setExtentZ(value_t ez) {
	const value_t center = (min.getZ() + max.getZ()) * 0.5;
	const value_t halfEx = ez * 0.5;
	min.setZ(center - halfEx);
	max.setZ(center + halfEx);
}
}

#endif /* GEOMETRY_BOX_H */
