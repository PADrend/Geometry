/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_BOXHELPER_H
#define GEOMETRY_BOXHELPER_H

#include "Definitions.h"
#include <vector>

namespace Geometry {

template <typename value_t>
class _Box;
using Box_f = _Box<float>;
template <typename value_t>
class _Matrix4x4;
using Matrix4x4f = _Matrix4x4<float>;
template <typename value_t>
class _Vec3;
using Vec3f = _Vec3<float>;

namespace Helper {

//! Return the indices of the corners for the requested @p side.
const corner_t * getCornerIndices(const side_t side);

//! Return the normal for the requested @p side.
const Vec3f & getNormal(const side_t side);

/**
 * Create new boxes by splitting up a box along x/y/z axis.
 *
 * @param box Box that will be used for splitting
 * @param partsX number of resulting boxes along x-axis
 * @param partsY number of resulting boxes along y-axis
 * @param partsZ number of resulting boxes along z-axis
 * @return Container with the new boxes
 */
std::vector<Box_f> splitUpBox(const Box_f & box, unsigned int partsX, unsigned int partsY, unsigned int partsZ);

/**
 * Split up a box once along up to three axis such that the resulting boxes are
 * getting cube-like (when repeating this splitting). A box is cube-like if
 * extentMax / extentMin <= sqrt(2).
 *
 * @param box Box that will be used for splitting
 * @return Container with the new boxes
 */
std::vector<Box_f> splitBoxCubeLike(const Box_f & box);

/**
 * Take a box and transform it by applying a transformation matrix to the
 * corners. The resulting box will be axis-aligned.
 *
 * @param box Box whose corners will be used
 * @param matrix Transformation matrix
 * @return Axis-aligned box including the transformed corners
 */
Box_f getTransformedBox(const Box_f & box, const Matrix4x4f & matrix);
}
}

#endif /* GEOMETRY_BOXHELPER_H */
