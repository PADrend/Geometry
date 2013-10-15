# Try to find CppUnit. Once done, this will define:
#
#   CPPUNIT_FOUND - variable which returns the result of the search
#   CPPUNIT_INCLUDE_DIRS - list of include directories
#   CPPUNIT_LIBRARIES - options for the linker

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_package(PkgConfig)
pkg_check_modules(PC_CPPUNIT QUIET cppunit)

find_path(CPPUNIT_INCLUDE_DIR
	cppunit/TestSuite.h
	HINTS ${PC_CPPUNIT_INCLUDEDIR} ${PC_CPPUNIT_INCLUDE_DIRS}
)
find_library(CPPUNIT_LIBRARY
	cppunit
	HINTS ${PC_CPPUNIT_LIBDIR} ${PC_CPPUNIT_LIBRARY_DIRS}
)

set(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_DIR})
set(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CppUnit DEFAULT_MSG
	CPPUNIT_INCLUDE_DIR
	CPPUNIT_LIBRARY
)

mark_as_advanced(
	CPPUNIT_INCLUDE_DIR
	CPPUNIT_LIBRARY
)
