/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BoundingSphere.h"
#include "Sphere.h"
#include "Vec3.h"
#include <cmath>
#include <cstring> /* for std::memcmp */
#include <limits>
#include <list>
#include <utility>

namespace Geometry {
namespace BoundingSphere {

/**
 * Calculate the excess of a point with respect to a sphere.
 * The sphere is defined by its center and its squared radius.
 *
 * @see Page 328: \f$ e := ||p - c||^2 - r^2\f$
 * @param center Center of the sphere
 * @param radiusSquared Squared radius of the sphere
 * @param point %Point to calculate the excess for
 * @return Excess of the point
 */
template <typename _T>
static _T calcExcess(const _Vec3<_T> & center, _T radiusSquared, const _Vec3<_T> & point) {
	return point.distanceSquared(center) - radiusSquared;
}

/**
 * Calculate the maximum excess of multiple points with respect to a sphere.
 * The sphere is defined by its center and its squared radius.
 * The points are given as the beginning and ending of a range.
 *
 * @see Algorithm 2 on Page 328
 * @param center Center of the sphere
 * @param radiusSquared Squared radius of the sphere
 * @param first Beginning of the range of points
 * @param last Ending of the range of points
 * @return Pair containing the maximum excess and an iterator pointing to the element with maximum excess
 */
template <typename InputIterator, typename _T>
static std::pair<_T, InputIterator> calcMaxExcess(const _Vec3<_T> & center, _T radiusSquared, InputIterator first,
												  InputIterator last) {
	_T maxExcess = std::numeric_limits<_T>::lowest();
	InputIterator result;
	for (; first != last; ++first) {
		const _T excess = calcExcess(center, radiusSquared, *first);
		if (excess > maxExcess) {
			maxExcess = excess;
			result = first;
		}
	}
	return std::make_pair(maxExcess, result);
}

/**
 * Move the given point to the front of the container.
 *
 * @param points Container holding points
 * @param point %Point that is to be moved to the front
 */
template <typename Container>
static void moveToFront(Container & points, typename Container::const_iterator point) {
	points.emplace_front(*point);
	points.erase(point);
}

/**
 * Storage of data for one execution of the primitive operation.
 * These data objects are stored in a stack.
 *
 * @see Section 4 on Page 329 and Page 335
 */
template <typename _T>
struct PrimitiveOperationData {
	_T z;
	_Vec3<_T> v;

	_Vec3<_T> center;
	_T radiusSquared;
};

/**
 * Storage of data that is needed during the execution of the algorithm.
 */
template <typename Container, typename _T>
struct AlgorithmData {
	//! Stack of miniball data calculated by mbBar()
	std::vector<PrimitiveOperationData<_T>> stack;

	//! End of the support set (see Page 327)
	typename Container::const_iterator s;

	//! Cache for the lastest valid center of the sphere
	_Vec3<_T> center;
	//! Cache for the latest valid squared radius of the sphere
	_T radiusSquared;
};

/**
 * Calculation of \f$\overline{\texttt{mb}}(B')\f$ for a new point \f$p\f$,
 * with \f$B' = B \cup \{p\}\f$, when \f$\overline{\texttt{mb}}(B)\f$ has been calculated already.
 *
 * @see Section 3 on Page 328f.
 * @param point New point \f$p\f$
 * @param data Data containing the result for \f$\overline{\texttt{mb}}(B)\f$
 * @return @c false if and only if the push operation should be rejected (see Equation 12 on Page 332)
 */
template <typename Container, typename _T>
static bool mbBar(const _Vec3<_T> & point, AlgorithmData<Container, _T> & data) {
	if (data.stack.empty()) {
		PrimitiveOperationData<_T> stackItem;
		stackItem.center = point;
		stackItem.radiusSquared = 0;
		data.stack.emplace_back(std::move(stackItem));
	} else {
		const size_t m = data.stack.size();
		PrimitiveOperationData<_T> current;
		const PrimitiveOperationData<_T> & prev = data.stack.back();

		// Page 329: Q_m := q_m - q_0
		// data.stack[0].center stores q0
		current.v = point - data.stack[0].center;

		// Page 334: \alpha_{m,i} = (2 / z_i) * (Q_i - \bar{Q}_i)^T * Q_m   i < m
		_Vec3<_T> alpha;
		for (size_t i = 1; i < m; ++i) {
			alpha[i] = (2 / data.stack[i].z) * data.stack[i].v.dot(current.v);
		}

		// Page 335: Store vector Q_m - \bar{Q}_m
		for (size_t i = 1; i < m; ++i) {
			current.v -= data.stack[i].v * alpha[i];
		}

		// Page 335: Store z_m
		// Lemma 1.iii: z = 2 * (Q_m - \bar{Q}_m)^T * (Q_m - \bar{Q}_m)
		current.z = 2 * current.v.dot(current.v);

		// Equation 12: Ignore push if z / r^2_{curr} < \epsilon
		const _T epsilon = 1.0e-32;
		if (current.z < epsilon * prev.radiusSquared) {
			return false;
		}

		// Lemma 1.iv:
		// c' = c + (e / z) * (Q_m - \bar{Q}_m)
		// r'^2 = r^2 + (e^2 / (2 * z))
		const _T excess = calcExcess(prev.center, prev.radiusSquared, point);
		const _T factor = excess / current.z;

		current.center = prev.center + current.v * factor;
		current.radiusSquared = prev.radiusSquared + factor * excess / 2;

		data.stack.emplace_back(std::move(current));
	}

	data.center = data.stack.back().center;
	data.radiusSquared = data.stack.back().radiusSquared;
	return true;
}

/**
 * Move-to-front miniball computation
 *
 * @see Algorithm 1 on Page 327
 */
template <typename Container, typename _T>
static void mtf_mb(Container & points, typename Container::const_iterator endPoint,
				   AlgorithmData<Container, _T> & data) {
	// Support set is empty
	data.s = points.cbegin();

	if (data.stack.size() == 4) {
		return;
	}
	for (auto it = points.cbegin(); it != endPoint;) {
		auto i = it++;
		// Check if *i is outside of the sphere
		if (calcExcess(data.center, data.radiusSquared, *i) > 0) {
			if (mbBar(*i, data)) {
				mtf_mb(points, i, data);
				data.stack.pop_back();
				// If i is the end of the support set, the support set is increased by one
				if (data.s == i) {
					++data.s;
				}
				moveToFront(points, i);
			}
		}
	}
}

/**
 * Pivot miniball computation
 *
 * @see Algorithm 2 on Page 328
 */
template <typename Container, typename _T>
static _Sphere<_T> pivot_mb(Container & points) {
	AlgorithmData<Container, _T> data;

	// Initialize the sphere with invalid values, which will generate
	// an excess greater than zero for any point.
	data.center = _Vec3<_T>(0, 0, 0);
	data.radiusSquared = std::numeric_limits<_T>::lowest();

	// t := 1
	auto t = std::next(points.cbegin());
	mtf_mb(points, t, data);
	_T maxExcess;
	_T oldRadiusSquared = std::numeric_limits<_T>::lowest();
	do {
		// Use t as beginning of range, to make sure k > t
		auto pair = calcMaxExcess(data.center, data.radiusSquared, t, points.cend());
		maxExcess = pair.first;
		const auto & k = pair.second;
		if (maxExcess > 0) {
			t = data.s;
			if (t == k) {
				std::advance(t, 1);
			}
			oldRadiusSquared = data.radiusSquared;
			mbBar(*k, data);
			mtf_mb(points, data.s, data);
			data.stack.pop_back();

			// If k is the end of the support set, the support set is increased by one
			if (data.s == k) {
				++data.s;
			}
			moveToFront(points, k);
		}
	} while (maxExcess > 0 && data.radiusSquared > oldRadiusSquared);
	return _Sphere<_T>(data.center, std::sqrt(data.radiusSquared));
}

static Sphere_f computeMiniballList(std::list<Vec3d> & pointList) {
	// Remove duplicate values from the list
	struct MemCompare {
		bool operator()(const Geometry::Vec3d & a, const Geometry::Vec3d & b) const {
			return std::memcmp(&a, &b, sizeof(Geometry::Vec3d)) < 0;
		}
	};
	pointList.sort(MemCompare());
	pointList.unique();

	// Use double values here, because float values become instable in some cases.
	const Sphere_d sphereDouble = pivot_mb<std::list<Vec3d>, double>(pointList);

	return Sphere_f(Vec3f(sphereDouble.getCenter()), sphereDouble.getRadius());
}

Sphere_f computeMiniball(const std::vector<Vec3f> & points) {
	// Use a list here, because firstly the original algorithm suggests it,
	// and secondly moving an element to the front is fastest for a list.
	std::list<Vec3d> pointList;
	for (const auto & p : points) {
		pointList.emplace_back(p);
	}

	return computeMiniballList(pointList);
}

/**
 * Project all points from a range onto a normal and identifiy the points with
 * the extremal projected values.
 *
 * @tparam value_t Value type (e.g. float, double)
 * @tparam iterator_t Iterator type for points
 * @tparam nx X coordinate value of the normal
 * @tparam ny Y coordinate value of the normal
 * @tparam nz Z coordinate value of the normal
 * @param first Iterator to begin of the point range
 * @param last Iterator past the end of the point range
 * @param indices Array of iterators to which two iterators pointing to the
 * extremal points will be added
 */
template <typename value_t, typename iterator_t, int nx, int ny, int nz>
inline static void projectToNormal(iterator_t first, const iterator_t & last, std::vector<iterator_t> & indices) {
	auto minValue = std::numeric_limits<value_t>::max();
	auto minPoint = last;
	auto maxValue = std::numeric_limits<value_t>::lowest();
	auto maxPoint = last;

	for (; first != last; ++first) {
		const auto & point = *first;
		// Project point onto normal
		const auto projection = nx * point.getX() + ny * point.getY() + nz * point.getZ();
		if (projection < minValue) {
			minValue = projection;
			minPoint = first;
		}
		if (projection > maxValue) {
			maxValue = projection;
			maxPoint = first;
		}
	}
	indices.emplace_back(minPoint);
	indices.emplace_back(maxPoint);
}

template <size_t numNormals>
std::list<Vec3d> findExtremalPoints(const std::vector<Vec3f> & points) {
	using it_t = std::vector<Vec3f>::const_iterator;

	std::vector<it_t> extremalIndices;
	extremalIndices.reserve(2 * numNormals);

	// See Page 28 for table of normals
	// type 0 0 1
	projectToNormal<float, it_t, 1, 0, 0>(points.cbegin(), points.cend(), extremalIndices);
	projectToNormal<float, it_t, 0, 1, 0>(points.cbegin(), points.cend(), extremalIndices);
	projectToNormal<float, it_t, 0, 0, 1>(points.cbegin(), points.cend(), extremalIndices);
	if (numNormals > 3) {
		// type 1 1 1
		projectToNormal<float, it_t, 1, 1, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 1, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -1, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -1, -1>(points.cbegin(), points.cend(), extremalIndices);
	}
	if (numNormals > 7) {
		// type 0 1 1
		projectToNormal<float, it_t, 1, 1, 0>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -1, 0>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 0, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 0, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 0, 1, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 0, 1, -1>(points.cbegin(), points.cend(), extremalIndices);
	}
	if (numNormals > 13) {
		// type 0 1 2
		projectToNormal<float, it_t, 0, 1, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 0, 2, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 0, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 0, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 2, 0>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 1, 0>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 0, 1, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 0, 2, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 0, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 0, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -2, 0>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -1, 0>(points.cbegin(), points.cend(), extremalIndices);
		// type 1 1 2
		projectToNormal<float, it_t, 1, 1, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 1, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 2, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -1, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 1, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -1, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -1, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 1, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -1, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -2, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 2, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -2, -1>(points.cbegin(), points.cend(), extremalIndices);
		// type 1 2 2
		projectToNormal<float, it_t, 2, 2, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 2, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 1, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -2, 1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 2, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -2, -1>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -2, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, 2, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 1, -2, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -1, 2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, 1, -2>(points.cbegin(), points.cend(), extremalIndices);
		projectToNormal<float, it_t, 2, -1, -2>(points.cbegin(), points.cend(), extremalIndices);
	}

	// Remove duplicate indices
	std::sort(extremalIndices.begin(), extremalIndices.end());
	extremalIndices.erase(std::unique(extremalIndices.begin(), extremalIndices.end()), extremalIndices.end());

	std::list<Vec3d> extremalPoints;
	for (const auto & index : extremalIndices) {
		extremalPoints.emplace_back(*index);
	}
	return extremalPoints;
}

template <size_t s>
static Sphere_f extremalPointsOptimalSphere(const std::vector<Vec3f> & points) {
	static_assert(s == 3 || s == 7 || s == 13 || s == 49, "s must be from {3, 7, 13, 49}");
	const size_t n = points.size();
	if (n > 2 * s) {
		auto extremalPoints = findExtremalPoints<s>(points);
		Sphere_f sphere = computeMiniballList(extremalPoints);
		for (const auto & point : points) {
			sphere.include(point);
		}
		return sphere;
	} else {
		return computeMiniball(points);
	}
}

Sphere_f computeEPOS6(const std::vector<Vec3f> & points) {
	return extremalPointsOptimalSphere<3>(points);
}

Sphere_f computeEPOS14(const std::vector<Vec3f> & points) {
	return extremalPointsOptimalSphere<7>(points);
}

Sphere_f computeEPOS26(const std::vector<Vec3f> & points) {
	return extremalPointsOptimalSphere<13>(points);
}

Sphere_f computeEPOS98(const std::vector<Vec3f> & points) {
	return extremalPointsOptimalSphere<49>(points);
}
}
}
