/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_BOUNDINGSPHERE_H
#define GEOMETRY_BOUNDINGSPHERE_H

#include <vector>

namespace Geometry {
// Forward declarations
template <typename _T>
class _Vec3;
using Vec3f = _Vec3<float>;
template <typename _T>
class _Sphere;
using Sphere_f = _Sphere<float>;

/**
 * @brief Bounding sphere computations
 *
 * Different algorithms for computing a bounding sphere for a given point set in three-dimensional space.
 *
 * @author Benjamin Eikel
 * @date 2012-03-20
 */
namespace BoundingSphere {

/**
 * Bounding sphere algorithm using the move-to-front heuristic.
 * Based on the article:
 * Bernd Gärtner: Fast and Robust Smallest Enclosing Balls.
 * Algorithms - ESA '99, LNCS 1643, pp. 325-338, Springer Berlin/Heidelberg, 1999.
 *
 * @note This implementation is based on the article only.
 * No code from the web was used for this implementation.
 * @author Benjamin Eikel
 * @date 2012-03-20
 */
Sphere_f computeMiniball(const std::vector<Vec3f> & points);

/**
 * @see computeEPOS98()
 * @note This version uses 3 normals
 */
Sphere_f computeEPOS6(const std::vector<Vec3f> & points);

/**
 * @see computeEPOS98()
 * @note This version uses 7 normals
 */
Sphere_f computeEPOS14(const std::vector<Vec3f> & points);

/**
 * @see computeEPOS98()
 * @note This version uses 13 normals
 */
Sphere_f computeEPOS26(const std::vector<Vec3f> & points);

/**
 * Bounding sphere algorithm using an extremal points heuristic.
 * Based on the article:
 * Thomas Larsson: Fast and Tight Fitting Bounding Spheres.
 * Proceedings of SIGRAD 2008, pp. 27-30, Linköping University Electronic Press, 2008.
 *
 * @note This version uses 49 normals
 * @author Benjamin Eikel
 * @date 2012-03-23
 */
Sphere_f computeEPOS98(const std::vector<Vec3f> & points);
}
}

#endif /* GEOMETRY_BOUNDINGSPHERE_H */
