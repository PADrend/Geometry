#
# This file is part of the Geometry library.
# Copyright (C) 2009-2015 Benjamin Eikel <benjamin@eikel.org>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.1.0)
project(Geometry VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up install directories
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKECONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Geometry)

# Add the library
add_library(Geometry
	BoundingSphere.cpp
	BoxHelper.cpp
	BoxIntersection.cpp
	Frustum.cpp
	RayBoxIntersection.cpp
	Tools.cpp
)
set_property(TARGET Geometry PROPERTY PUBLIC_HEADER
	Angle.h
	BoundingSphere.h
	Box.h
	BoxHelper.h
	BoxIntersection.h
	Convert.h
	Definitions.h
	DualQuaternion.h
	Frustum.h
	Interpolation.h
	Line.h
	LineTriangleIntersection.h
	Matrix3x3.h
	Matrix4x4.h
	Plane.h
	Point.h
	PointOctree.h
	Quaternion.h
	RayBoxIntersection.h
	Rect.h
	Sphere.h
	SRT.h
	Tetrahedron.h
	Tools.h
	Triangle.h
	TriangleTriangleIntersection.h
	Vec2.h
	Vec3.h
	Vec4.h
	VecHelper.h
	VecN.h
)

# Set version of library
set_target_properties(Geometry PROPERTIES VERSION ${Geometry_VERSION}
                                          SOVERSION ${Geometry_VERSION_MAJOR})

option(GEOMETRY_BUILD_TESTS "Defines if CppUnit tests for the Geometry library are built.")
if(GEOMETRY_BUILD_TESTS)
	if(UNIX AND NOT APPLE)
	        find_program(MEMORYCHECK_COMMAND NAMES valgrind)
		set(MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leak-check=summary --num-callers=1 --vgdb=no")
	endif()
	include(CTest)
	add_subdirectory(tests)
endif()

# Install the library
install(TARGETS Geometry EXPORT LibraryExport
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtimelibraries
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtimelibraries
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT developmentlibraries
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Geometry COMPONENT headers
)

# Set include directories for users of this library
target_include_directories(Geometry
                           INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/.."
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Library export
install(EXPORT LibraryExport DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} FILE GeometryTargets.cmake COMPONENT developmentlibraries)

# Create the configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(GeometryConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/GeometryConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR}
                              PATH_VARS CMAKE_INSTALL_CMAKECONFIGDIR
                              NO_SET_AND_CHECK_MACRO
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/GeometryConfigVersion.cmake
                                 COMPATIBILITY SameMajorVersion)

# Install the configuration files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/GeometryConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/GeometryConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} COMPONENT developmentlibraries)

# Packaging
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "libgeometry${Geometry_VERSION_MAJOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Geometry library")
set(CPACK_PACKAGE_VENDOR "Benjamin Eikel; Claudius Jaehn; Ralf Petring")
set(CPACK_PACKAGE_CONTACT "Benjamin Eikel <benjamin@eikel.org>")
set(CPACK_PACKAGE_VERSION_MAJOR ${Geometry_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Geometry_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Geometry_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "libGeometry")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_STRIP_FILES ON)

include(CPack)

