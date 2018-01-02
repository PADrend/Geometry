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
enum class side_t { X_NEG = 0, Y_NEG = 1, Z_NEG = 2, X_POS = 3, Y_POS = 4, Z_POS = 5 };

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
enum class corner_t {
	xyz = 0,
	Xyz = 1,
	xYz = 2,
	XYz = 3,
	xyZ = 4,
	XyZ = 5,
	xYZ = 6,
	XYZ = 7
};

//! Enumeration for at most three dimensions.
enum class dimension_t { X = 0, Y = 1, Z = 2 };

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
enum class rectCorner_t { xy = 0, Xy = 1, xY = 2, XY = 3 };
}

#endif /* GEOMETRY_DEFINITIONS_H */
