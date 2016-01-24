/*
	This file is part of the Geometry library.
	Copyright (C) 2012 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Box.h"
#include "Frustum.h"
#include "Line.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Plane.h"
#include "Point.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Sphere.h"
#include "SRT.h"
#include "Tetrahedron.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "VecN.h"
#include <type_traits>

static_assert(std::is_standard_layout<Geometry::_Line<Geometry::_Vec3<float>>>::value,
			  "Geometry::_Line is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Line<Geometry::_Vec3<double>>>::value,
			  "Geometry::_Line is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Matrix3x3<float>>::value,
			  "Geometry::_Matrix3x3 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Matrix3x3<double>>::value,
			  "Geometry::_Matrix3x3 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Matrix4x4<float>>::value,
			  "Geometry::_Matrix4x4 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Matrix4x4<double>>::value,
			  "Geometry::_Matrix4x4 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Plane<float>>::value, "Geometry::_Plane is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Plane<double>>::value, "Geometry::_Plane is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Ray<Geometry::_Vec3<float>>>::value,
			  "Geometry::_Ray is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Ray<Geometry::_Vec3<double>>>::value,
			  "Geometry::_Ray is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Rect<float>>::value, "Geometry::_Rect is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Rect<double>>::value, "Geometry::_Rect is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Segment<Geometry::_Vec3<float>>>::value,
			  "Geometry::_Segment is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Segment<Geometry::_Vec3<double>>>::value,
			  "Geometry::_Segment is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Sphere<float>>::value,
			  "Geometry::_Sphere is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Sphere<double>>::value,
			  "Geometry::_Sphere is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_SRT<float>>::value, "Geometry::_SRT is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_SRT<double>>::value, "Geometry::_SRT is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec2<float>>::value, "Geometry::_Vec2 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec2<double>>::value, "Geometry::_Vec2 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec3<float>>::value, "Geometry::_Vec3 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec3<double>>::value, "Geometry::_Vec3 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec4<float>>::value, "Geometry::_Vec4 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_Vec4<double>>::value, "Geometry::_Vec4 is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_VecN<float>>::value, "Geometry::_VecN is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::_VecN<double>>::value, "Geometry::_VecN is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Box>::value, "Geometry::Box is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Frustum>::value, "Geometry::Frustum is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Point<Geometry::_Vec3<float>>>::value,
			  "Geometry::Point is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Point<Geometry::_Vec3<double>>>::value,
			  "Geometry::Point is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Quaternion>::value, "Geometry::Quaternion is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Tetrahedron<float>>::value,
			  "Geometry::Tetrahedron is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Tetrahedron<double>>::value,
			  "Geometry::Tetrahedron is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Triangle<Geometry::_Vec3<float>>>::value,
			  "Geometry::Triangle is no standard-layout class");
static_assert(std::is_standard_layout<Geometry::Triangle<Geometry::_Vec3<double>>>::value,
			  "Geometry::Triangle is no standard-layout class");
