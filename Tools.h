/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_TOOLS_H
#define GEOMETRY_TOOLS_H

#include "Line.h"
#include "Plane.h"
#include "Vec3.h"
#include "Vec4.h"
#include <utility>
#include <tuple>

#if defined(__GNUC__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: DEPRECATED not set for this compiler")
#define DEPRECATED
#endif

namespace Geometry {
template <typename value_t>
class _Box;
typedef _Box<float> Box;
class Frustum;
template <typename _T>
class _Matrix4x4;
typedef _Matrix4x4<float> Matrix4x4f;
template <typename _T>
class _Rect;
typedef _Rect<float> Rect_f;
template <typename T_>
class _Sphere;
typedef _Sphere<float> Sphere_f;

// note:  planeNormal (dot) (  (rayPos-planePos) + i * rayDir) = 0
// -> Si = planeNormal  (dot) (rayPos-planePos) / (planeNormal  (dot) rayDir )
inline bool rayPlaneIntersection(const Vec3f & planePos, const Vec3f & planeNormal, const Vec3f & rayPos,
								 const Vec3f & rayDir, Vec3f & intersectionPoint) {
	return Plane(planePos, planeNormal).getIntersection(Line3(rayPos, rayDir), intersectionPoint);
}

DEPRECATED inline bool rayPlaneIntersection(const Plane & plane, const Ray3f & ray, Vec3f & intersectionPoint);

bool rayPlaneIntersection(const Plane & plane, const Ray3f & ray, Vec3f & intersectionPoint) {
	return plane.getIntersection(ray, intersectionPoint);
}

/*! (intersects?, lineIntersectionValue1, lineIntersectionValue2) */
std::tuple<bool, float, float> normLineSphereIntersections(const Line3f & normalizedLine, const Sphere_f & sphere);
std::tuple<bool, float, float> lineSphereIntersections(const Line3f & line, const Sphere_f & sphere);

/**
 * Calculate a pair of points on the given lines @a lineA and @a lineB that is closest to each other.
 *
 * @param lineA The first line
 * @param lineB The second line
 * @param parameterA Return value that can be used to calculate the point on @a lineA.
 * @param parameterB Return value that can be used to calculate the point on @a lineB.
 * @note The function @a _Line<_T>::getPoint can be used with the returned parameters as argument to calculate the
 *points.
 * @author Benjamin Eikel
 * @date 2010-10-14
 */
template <typename float_t>
void getClosestPoints(const _Line<_Vec3<float_t>> & lineA, const _Line<_Vec3<float_t>> & lineB, float_t & parameterA,
					  float_t & parameterB) {
	std::pair<float_t, float_t> result = lineA.getClosestValues(lineB);
	parameterA = result.first;
	parameterB = result.second;
}

/**
 * Calculate a pair of points on the given line @a line and ray @a ray that is closest to each other.
 *
 * @param line The line
 * @param ray The ray
 * @param parameterA Return value that can be used to calculate the point on @a line.
 * @param parameterB Return value that can be used to calculate the point on @a ray.
 * @note The function @a _Line<_T>::getPoint and @a _Ray<_T>::getPoint can be used with the returned parameters as
 *argument to calculate the points.
 * @author Benjamin Eikel
 * @date 2010-10-14
 */
template <typename float_t>
void getClosestPoints(const _Line<_Vec3<float_t>> & line, const _Ray<_Vec3<float_t>> & ray, float_t & parameterA,
					  float_t & parameterB) {
	std::pair<float_t, float_t> result = line.getClosestValues(ray);
	parameterA = result.first;
	parameterB = result.second;
}

/**
 * Returns the point on the given line that is closest to the given ray.
 *
 * @param linesPos Origin of the line
 * @param lineDir Direction vector of the line
 * @param rayPos Origin of the ray
 * @param rayDir Direction of the ray
 * @return Point on the line that is closest to the ray
 */
template <typename float_t>
inline _Vec3<float_t> getClosestPointOnLineToRay(const _Vec3<float_t> & linePos, const _Vec3<float_t> & lineDir,
												 const _Vec3<float_t> & rayPos, const _Vec3<float_t> & rayDir) {
	const _Line<_Vec3<float_t>> line(linePos, lineDir);
	const _Ray<_Vec3<float_t>> ray(rayPos, rayDir);
	return line.getPoint(line.getClosestValues(ray).first);
}

/**
 * Map object coordinates to window coordinates.
 *
 * @param obj Object coordinates
 * @param worldToScreen Transformation from world space to screen space (e.g.,
 * @code projection * modelView @endcode)
 * @param viewport Viewport
 * @return Window coordinates. If the mapping failed, a zero vector is returned.
 * @see gluProject
 */
template <typename _T>
inline _Vec3<_T> project(const _Vec3<_T> & obj, const _Matrix4x4<_T> & worldToScreen, const _Rect<_T> & viewport) {
	const _T one = static_cast<_T>(1.0);
	const _T two = static_cast<_T>(2.0);
	const _Vec4<_T> v(obj, one);
	_Vec4<_T> v2 = worldToScreen * v;
	const _T w = v2.getW();
	if (w == static_cast<_T>(0.0)) {
		return _Vec3<_T>();
	}
	v2 /= w;
	_Vec3<_T> win;
	win.setX(viewport.getX() + (viewport.getWidth() * (v2.getX() + one)) / two);
	win.setY(viewport.getY() + (viewport.getHeight() * (v2.getY() + one)) / two);
	win.setZ((v2.getZ() + one) / two);
	return win;
}

/**
 * Map window coordinates to object coordinates.
 *
 * @param win Window coordinates
 * @param worldToScreen Transformation from world space to screen space (e.g.,
 * @code projection * modelView @endcode)
 * @param viewport Viewport
 * @return Object coordinates. If the mapping failed, a zero vector is returned.
 * @see gluUnProject
 */
template <typename _T>
inline _Vec3<_T> unProject(const _Vec3<_T> & win, const _Matrix4x4<_T> & worldToScreen, const _Rect<_T> & viewport) {
	const _T one = static_cast<_T>(1.0);
	const _T two = static_cast<_T>(2.0);
	_Vec4<_T> v;
	v.setX((two * (win.getX() - viewport.getX())) / viewport.getWidth() - one);
	v.setY((two * (win.getY() - viewport.getY())) / viewport.getHeight() - one);
	v.setZ(two * win.getZ() - one);
	v.setW(one);
	_Vec4<_T> obj = worldToScreen.inverse() * v;
	const _T w = obj.getW();
	if (w == static_cast<_T>(0.0)) {
		return _Vec3<_T>();
	}
	return _Vec3<_T>(obj.getX() / w, obj.getY() / w, obj.getZ() / w);
}

/**
 * Map a box in object coordinates to a rect in window coordinates.
 *
 * @param box Box that is to be mapped
 * @param modelView Model view matrix
 * @param projection Projection matrix
 * @param viewport Viewport
 * @return Rect in window coordinates
 */
Rect_f projectBox(const Box & box, const Matrix4x4f & modelView, const Matrix4x4f & projection,
				  const Rect_f & viewport);

/**
 * Calculate an orthographic frustum that encloses the given box when seen by an observer.
 *
 * @param box Box that should be enclosed
 * @param modelView Matrix that is used to transform the box
 * @return Orthographic frustum
 * @note If the box is behind the observer, the frustum's near and far values will be negative.
 * @note Example 1: @a box can be given in world coordinates,
 * and @a modelView contains a camera matrix.
 * @note Example 2: @a box can be given in object coordinates,
 * and @a modelView contains the product of a camera matrix and the transformation matrix of the object.
 */
Frustum calcEnclosingOrthoFrustum(const Box & box, const Matrix4x4f & modelView);
}

#endif /* GEOMETRY_TOOLS_H */
