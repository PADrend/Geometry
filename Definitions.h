/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_DEFINITIONS_H
#define GEOMETRY_DEFINITIONS_H

namespace Geometry {

/**
 * Sides of a three-dimensional box.
 @verbatim
		^Y
		|
		|
		|
		o---> X
	   /
	Z /    .---------.
		  / Y_POS:  /|
		 /    4    / |--Z_NEG: 2
		.---------.  |
 X_NEG: |         | ----X_POS: 3
	0---|  Z_POS: |  |
		|    5    | /
		|         |/--Y_NEG: 1
		'---------'
 @endverbatim
 * @note The mapping to integers must not be changed because some functions depend on it for iteration.
 */
enum side_t { SIDE_X_NEG = 0, SIDE_Y_NEG = 1, SIDE_Z_NEG = 2, SIDE_X_POS = 3, SIDE_Y_POS = 4, SIDE_Z_POS = 5 };

/**
 * Corners of a three-dimensional box.
 @verbatim
	   ^Y
	   |
	   |
	   |
	   o---> X
	  /
	Z/

 Corners:
	  2---------3
	 /|        /|
	/ |       / |
   6---------7  |
   |  |      |  |
   |  0------|--1
   | /       | /
   |/        |/
   4---------5
 @endverbatim
 * @note The mapping to integers must not be changed because some functions depend on it.
 * 			- Box::getOctant(Vec3f v)
 * 			- Box::getOppositeCorner(cornet_t nr)
 *
 * @note Meaning of the variable names:
 * 		small letters denote corners in negative side of the cube,
 * 		great letters denote corners in positive side of the cube
 */
enum corner_t {
	CORNER_xyz = 0,
	CORNER_Xyz = 1,
	CORNER_xYz = 2,
	CORNER_XYz = 3,
	CORNER_xyZ = 4,
	CORNER_XyZ = 5,
	CORNER_xYZ = 6,
	CORNER_XYZ = 7
};

//! Enumeration for at most three dimensions.
enum dimension_t { X_DIMENSION = 0, Y_DIMENSION = 1, Z_DIMENSION = 2 };

/**
 * Corners of a two-dimensional rect.
 @verbatim
	   ^Y
	   |
	   |
	   |
	   o---> X


 Corners:
	2---------3
	|         |
	|         |
	|         |
	0---------1

 @endverbatim
 * @note The mapping to integers must not be changed because some functions depend on it.
 *
 * @note Meaning of the variable names:
 * 		small letters denote corners in negative side of the rect,
 * 		great letters denote corners in positive side of the rect
 */
enum rectCorner_t { CORNER_xy = 0, CORNER_Xy = 1, CORNER_xY = 2, CORNER_XY = 3 };
}

#endif /* GEOMETRY_DEFINITIONS_H */
