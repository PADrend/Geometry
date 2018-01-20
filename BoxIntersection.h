/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_BOXINTERSECTION_H
#define GEOMETRY_BOXINTERSECTION_H

namespace Geometry {

template <typename value_t>
class _Box;
using Box_f = _Box<float>;
template <typename T_>
class _Vec3;
template <typename T_>
class Triangle;
using Triangle_f = Triangle<_Vec3<float>>;

namespace Intersection {

/**
 * Check if a box intersect with a triangle.

\see based on the following paper:
	@article{AkenineMoller01,
		author = "Tomas Akenine-M�ller",
		title = "Fast 3D Triangle-Box Overlap Testing",
		journal = "journal of graphics, gpu, and game tools",
		volume = "6",
		number = "1",
		pages = "29-33",
		year = "2001",
	}

\see original source code:
	http://jgt.akpeters.com/papers/AkenineMoller01/tribox.html
*/
bool isBoxIntersectingTriangle(const Box_f & box, const Triangle_f & triangle);

/**
 * Check if two boxes intersect.
 *
 * @param a First box
 * @param b Second box
 */
template <class box_t>
bool isBoxIntersectingBox(const box_t & a, const box_t & b) {
	return b.getMinX() < a.getMaxX() && b.getMaxX() > a.getMinX() && b.getMinY() < a.getMaxY()
			&& b.getMaxY() > a.getMinY() && b.getMinZ() < a.getMaxZ() && b.getMaxZ() > a.getMinZ();
}

/**
 * Calculate the intersection of two boxes.
 *
 * @param a First box
 * @param b Second box
 * @return Intersection of the two boxes
 */
template <class box_t>
box_t getBoxBoxIntersection(const box_t & a, const box_t & b) {
	box_t result;
	result.setMin(box_t::vec3_t::pairwiseMax(a.getMin(), b.getMin()));
	result.setMax(box_t::vec3_t::pairwiseMin(a.getMax(), b.getMax()));
	return result;
}
}
}

#endif /* GEOMETRY_BOXINTERSECTION_H */
