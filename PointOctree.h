/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_POINTOCTREE_H
#define GEOMETRY_POINTOCTREE_H

#include "Box.h"
#include "BoxHelper.h"
#include "BoxIntersection.h"
#include "Definitions.h"
#include "Sphere.h"
#include "Vec3.h"
#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <stack>
#include <cstdint>
#include <vector>

namespace Geometry {

/**
 * Three-dimensional spatial data structure for storing points with additional arbitrary data.
 *
 * @author Benjamin Eikel, Jonas Knoll
 * @date 2011-02-08
 */
template <typename Point_t>
class PointOctree {
private:
	float minBoxSize; //!< Lower bound for side length of cell boundary.
	uint32_t maxNumPoints; //!< Upper bound for number of points inside a leaf cell.

	Box box; //!< Bounding box of octree cell.
	using children_t = std::vector<PointOctree<Point_t>>;
	using children_iterator_t = typename children_t::iterator;
	using children_const_iterator_t = typename children_t::const_iterator;
	children_t children; //!< vector of child nodes.
	std::deque<Point_t> points; //!< Points that are stored directly inside this node.

	/**
	 * Sphere box intersection test based on Arvo's algorithm.
	 *
	 * @param sphere the sphere
	 * @param box box to test intersection with
	 * @return @c true if sphere and box intersect, @c false otherwise.
	 */
	inline static bool sphereBoxIntersection(const Sphere_f & sphere, const Box & box);

public:
	using point_t = Point_t;

	/**
	 * Create a new octree for points within the given bounds.
	 *
	 * @param boundingBox Bounding box for all points to store.
	 * @param minimumBoxSize Minimum side length of leaf cells. If this is reached, the leaf will not be split anymore.
	 * @param maximumPoints Maximum number of points in leaf cells. If this is reached, a leaf will be split.
	 */
	PointOctree(const Box & boundingBox, float minimumBoxSize, uint32_t maximumPoints)
			: minBoxSize(minimumBoxSize), maxNumPoints(maximumPoints), box(boundingBox), children() {
	}

	const Box & getBox() const {
		return box;
	}
	float getMinBoxSize() const {
		return minBoxSize;
	}
	uint32_t getMaxNumPoints() const {
		return maxNumPoints;
	}
	bool hasChildren() const {
		return !children.empty();
	}
	bool isLeaf() const {
		return children.empty();
	}
	bool empty() const {
		return children.empty() && points.empty();
	}

	/**
	 * Delete all child nodes and all points.
	 */
	void clear() {
		children.clear();
		points.clear();
	}

	/**
	 * Insert the point into the octree.
	 *
	 * @param point Data item containing the position.
	 */
	inline bool insert(const Point_t & point);

	/**
	 * Return all points.
	 *
	 * @param out Points in the tree.
	 */
	inline void collectPoints(std::deque<Point_t> & out) const;

	/**
	 * Return all points where the location is within the given box.
	 *
	 * @param box
	 * @param out Points that fulfill the condition are added to this container.
	 */
	inline void collectPointsWithinBox(const Box & box, std::deque<Point_t> & out) const;

	/**
	 * Return all points where the location is within the sphere.
	 *
	 * @param sphere The sphere describing the query region.
	 * @param out Points that fulfill the condition are added to this container.
	 */
	inline void collectPointsWithinSphere(const Sphere_f & sphere, std::deque<Point_t> & out) const;

	/**
	 * Return the leaf node containing the given point or nullptr if the point is outside the tree.
	 *
	 * @param point Point
	 * @return PointOctree leaf node or nullptr.
	 */
	const PointOctree * findLeafCell(const Vec3f & point) const {
		bool found = box.contains(point);
		const PointOctree * cell = this;
		while (found && !cell->isLeaf()) {
			found = false;
			for (const auto & i : cell->children) {
				if (i.getBox().contains(point)) {
					cell = &i;
					found = true;
					break;
				}
			}
		}
		return found ? cell : nullptr;
	}
	PointOctree * findLeafCell(const Vec3f & point) {
		bool found = box.contains(point);
		PointOctree * cell = this;
		while (found && !cell->isLeaf()) {
			found = false;
			for (auto & i : cell->children) {
				if (i.getBox().contains(point)) {
					cell = &i;
					found = true;
					break;
				}
			}
		}
		return found ? cell : nullptr;
	}

	void getClosestPoints(const Vec3f & pos, size_t count, std::deque<Point_t> & out) const;

	std::deque<Point_t> getSortedClosestPoints(const Vec3f & pos, size_t count) const;
};

template <typename Point_t>
inline bool PointOctree<Point_t>::insert(const Point_t & point) {
	// make sure point is within boundary
	if (!box.contains(point.getPosition())) {
		return false;
	} else if (isLeaf()) {
		// add new point
		points.push_back(point);

		// need to split?
		if (points.size() > maxNumPoints && box.getExtentMax() >= minBoxSize * 2.0) {
			const auto newBoxes = Helper::splitBoxCubeLike(box);
			children.reserve(newBoxes.size());
			for (const auto & newBox : newBoxes) {
				children.emplace_back(newBox, minBoxSize, maxNumPoints);
			}

			// distribute existing points to new children
			for (const auto & currentPoint : points) {
				for (auto & i : children) {
					if (i.insert(currentPoint)) {
						break;
					}
				}
			}
			points.clear();
		}
		return true;
	} else {
		PointOctree * leaf = findLeafCell(point.getPosition());
		return leaf ? leaf->insert(point) : false;
	}
}

template <typename Point_t>
inline void PointOctree<Point_t>::collectPoints(std::deque<Point_t> & out) const {
	std::stack<const PointOctree *> activeCells;
	activeCells.push(this);
	while (!activeCells.empty()) {
		const PointOctree * cell = activeCells.top();
		activeCells.pop();
		if (cell->hasChildren()) {
			for (const auto & i : cell->children) {
				activeCells.push(&i);
			}
		} else {
			for (const auto & p : cell->points) {
				out.push_back(p);
			}
		}
	}
}

template <typename Point_t>
inline void PointOctree<Point_t>::collectPointsWithinBox(const Box & queryBox, std::deque<Point_t> & out) const {
	std::stack<const PointOctree *> activeCells;
	activeCells.push(this);
	while (!activeCells.empty()) {
		const PointOctree * cell = activeCells.top();
		activeCells.pop();

		if (!Intersection::isBoxIntersectingBox(cell->getBox(), queryBox)) {
			continue;
		} else if (queryBox.contains(cell->getBox())) {
			cell->collectPoints(out);
		} else if (cell->hasChildren()) {
			for (const auto & i : cell->children) {
				activeCells.push(&i);
			}
		} else {
			for (const auto & p : cell->points) {
				if (queryBox.contains(p.getPosition())) {
					out.push_back(p);
				}
			}
		}
	}
}

template <typename Point_t>
inline void PointOctree<Point_t>::collectPointsWithinSphere(const Sphere_f & sphere, std::deque<Point_t> & out) const {
	std::stack<const PointOctree *> activeCells;
	activeCells.push(this);

	const float radius0_5 = 0.5f * sphere.getRadius();
	while (!activeCells.empty()) {
		const PointOctree * cell = activeCells.top();
		activeCells.pop();
		if (sphereBoxIntersection(sphere, cell->getBox())) {
			if (cell->getBox().getExtentMax() < radius0_5) { // small box
				bool inSphere = true;
				for (uint_fast8_t i = 0; i < 8; ++i) { // completely inside sphere
					if (sphere.isOutside(cell->getBox().getCorner(static_cast<corner_t>(i)))) {
						inSphere = false;
						break;
					}
				}
				if (inSphere) {
					cell->collectPoints(out);
					continue;
				}
			}
			if (cell->isLeaf()) {
				for (const auto & p : cell->points) {
					if (!sphere.isOutside(p.getPosition())) {
						out.push_back(p);
					}
				}
			} else {
				// inner node
				for (const auto & i : cell->children) {
					activeCells.push(&i);
				}
			}
		}
	}
}

template <typename Point_t>
inline bool PointOctree<Point_t>::sphereBoxIntersection(const Sphere_f & sphere, const Box & box) {
	float dist = 0.0f;
	// find the square of the distance from the sphere to the box
	if (sphere.getCenter().x() < box.getMinX()) {
		const float s = sphere.getCenter().x() - box.getMinX();
		dist += s * s;
	} else if (sphere.getCenter().x() > box.getMaxX()) {
		const float s = sphere.getCenter().x() - box.getMaxX();
		dist += s * s;
	}
	if (sphere.getCenter().y() < box.getMinY()) {
		const float s = sphere.getCenter().y() - box.getMinY();
		dist += s * s;
	} else if (sphere.getCenter().y() > box.getMaxY()) {
		const float s = sphere.getCenter().y() - box.getMaxY();
		dist += s * s;
	}
	if (sphere.getCenter().z() < box.getMinZ()) {
		const float s = sphere.getCenter().z() - box.getMinZ();
		dist += s * s;
	} else if (sphere.getCenter().z() > box.getMaxZ()) {
		const float s = sphere.getCenter().z() - box.getMaxZ();
		dist += s * s;
	}
	return dist <= sphere.getRadius() * sphere.getRadius();
}

template <typename Point_t>
inline void PointOctree<Point_t>::getClosestPoints(const Vec3f & pos, size_t count, std::deque<Point_t> & out) const {
	const PointOctree * leaf = findLeafCell(pos);
	if (leaf == nullptr) {
		return;
	}
	if (count == 0) {
		out.clear();
		return;
	}
	const float maxRadius = box.getDiameter();
	for (float radius = leaf->box.getDiameter() * 0.25; radius <= maxRadius; radius *= 2.0) {
		out.clear();
		collectPointsWithinSphere(Sphere_f(pos, radius), out);
		if (out.size() >= count)
			break;
	}
	if (out.size() > count) {
		std::vector<std::pair<float, size_t>> sortedPoints;

		// store (distance,pointIndex) in sortedPoints
		size_t i = 0;
		for (auto & point : out) {
			sortedPoints.emplace_back(pos.distanceSquared(point.getPosition()), i++);
		}
		std::nth_element(sortedPoints.begin(), std::next(sortedPoints.begin(), (count - 1)), sortedPoints.end());
		// keep only the 'count'-many closest points
		sortedPoints.erase(std::next(sortedPoints.begin(), count), sortedPoints.end());

		// move the closest points to 'out'
		std::deque<Point_t> result;
		for (const auto & distancePointPair : sortedPoints) {
			result.emplace_back(out[distancePointPair.second]);
		}
		using std::swap;
		swap(out, result);
	}
}

template <typename Point_t>
inline std::deque<Point_t> PointOctree<Point_t>::getSortedClosestPoints(const Vec3f & pos, size_t count) const {
	std::deque<Point_t> closestPoints;
	getClosestPoints(pos, count, closestPoints);

	std::vector<std::pair<float, size_t>> sortingData;

	// store (distance,pointIndex) in sortingData
	size_t i = 0;
	for (auto & point : closestPoints)
		sortingData.emplace_back(pos.distanceSquared(point.getPosition()), i++);

	std::sort(sortingData.begin(), sortingData.end());

	std::deque<Point_t> sortedClosestPoints;
	for (const auto & distancePointPair : sortingData)
		sortedClosestPoints.emplace_back(closestPoints[distancePointPair.second]);

	return sortedClosestPoints;
}
}

#endif /* GEOMETRY_POINTOCTREE_H */
