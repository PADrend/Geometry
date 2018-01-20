#
# This file is part of the Geometry library.
# Copyright (C) 2010-2015 Benjamin Eikel <benjamin@eikel.org>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.1.0)

if(GEOMETRY_BUILD_TESTS)
	add_executable(GeometryTest
		BoundingSphereTest.cpp
		BoxTest.cpp
		ConvertTest.cpp
		FrustumTest.cpp
		InterpolationTest.cpp
		LineTest.cpp
		LineTriangleIntersectionTest.cpp
		Matrix4x4Test.cpp
		PlaneTest.cpp
		PointOctreeTest.cpp
		QuaternionTest.cpp
		RayBoxIntersectionTest.cpp
		RectTest.cpp
		SphereTest.cpp
		StandardLayout.cpp
		TetrahedronTest.cpp
		TriangleTest.cpp
		Vec3Test.cpp
		VecHelperTest.cpp
		VecNTest.cpp
		GeometryTestMain.cpp
	)

	target_link_libraries(GeometryTest PRIVATE Geometry)

	# Dependency to CppUnit
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
	find_package(CppUnit REQUIRED)
	target_include_directories(GeometryTest PRIVATE ${CPPUNIT_INCLUDE_DIRS})
	target_link_libraries(GeometryTest PRIVATE ${CPPUNIT_LIBRARIES})

	install(TARGETS GeometryTest
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT tests
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT tests
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT tests
	)

	add_test(NAME BoundingSphereTest COMMAND GeometryTest BoundingSphereTest)
	add_test(NAME BoxTest COMMAND GeometryTest BoxTest)
	add_test(NAME ConvertTest COMMAND GeometryTest ConvertTest)
	add_test(NAME FrustumTest COMMAND GeometryTest FrustumTest)
	add_test(NAME InterpolationTest COMMAND GeometryTest InterpolationTest)
	add_test(NAME LineTest COMMAND GeometryTest LineTest)
	add_test(NAME LineTriangleIntersectionTest COMMAND GeometryTest LineTriangleIntersectionTest)
	add_test(NAME Matrix4x4Test COMMAND GeometryTest Matrix4x4Test)
	add_test(NAME PlaneTest COMMAND GeometryTest PlaneTest)
	add_test(NAME PointOctreeTest COMMAND GeometryTest PointOctreeTest)
	add_test(NAME QuaternionTest COMMAND GeometryTest QuaternionTest)
	add_test(NAME RayBoxIntersectionTest COMMAND GeometryTest RayBoxIntersectionTest)
	add_test(NAME RectTest COMMAND GeometryTest RectTest)
	add_test(NAME SphereTest COMMAND GeometryTest SphereTest)
	add_test(NAME TetrahedronTest COMMAND GeometryTest TetrahedronTest)
	add_test(NAME TriangleTest COMMAND GeometryTest TriangleTest)
	add_test(NAME Vec3Test COMMAND GeometryTest Vec3Test)
	add_test(NAME VecHelperTest COMMAND GeometryTest VecHelperTest)
	add_test(NAME VecNTest COMMAND GeometryTest VecNTest)
endif()
