#
# This file is part of the Geometry library.
# Copyright (C) 2010-2013 Benjamin Eikel <benjamin@eikel.org>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 2.8.11)

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

	target_link_libraries(GeometryTest LINK_PRIVATE Geometry)

	# Dependency to CppUnit
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
	find_package(CppUnit REQUIRED)
	target_include_directories(GeometryTest PRIVATE ${CPPUNIT_INCLUDE_DIRS})
	target_link_libraries(GeometryTest LINK_PRIVATE ${CPPUNIT_LIBRARIES})

	if(COMPILER_SUPPORTS_CXX11)
		set_property(TARGET GeometryTest APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++11 ")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set_property(TARGET GeometryTest APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++0x ")
	endif()

	install(TARGETS GeometryTest
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT tests
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT tests
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT tests
	)

	add_test(NAME GeometryTest COMMAND GeometryTest)
endif()
