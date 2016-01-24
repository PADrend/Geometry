/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "BoxHelper.h"
#include "Box.h"
#include "Matrix4x4.h"
#include <array>
#include <vector>

namespace Geometry {
namespace Helper {

//
//	^Y
//	    |
//	    |
//	    |
//	    o---> X
//	   /
//	Z /      .---------.
//          /  Top:   /|
//         /    4    / |--Back: 2
//        .---------.  |
//  Left: |         | ----Right: 3
//    0---|  Front: |  |
//        |    5    | /
//        |         |/--Bottom: 1
//        '---------'
//           2---------3
//          /|        /|
//         / |       / |
//        6---------7  |
//        |  |      |  |
//        |  0------|--1
//        | /       | /
//        |/        |/
//        4---------5
//
//	CORNER_xyz = 0,
//	CORNER_Xyz = 1,
//	CORNER_xYz = 2,
//	CORNER_XYz = 3,
//	CORNER_xyZ = 4,
//	CORNER_XyZ = 5,
//	CORNER_xYZ = 6,
//	CORNER_XYZ = 7
const corner_t * getCornerIndices(const side_t side) {
	static const corner_t corners[6][4] = {
			{CORNER_xyz, CORNER_xyZ, CORNER_xYZ, CORNER_xYz}, // left
			{CORNER_XyZ, CORNER_xyZ, CORNER_xyz, CORNER_Xyz}, // bottom
			{CORNER_Xyz, CORNER_xyz, CORNER_xYz, CORNER_XYz}, // back
			{CORNER_XyZ, CORNER_Xyz, CORNER_XYz, CORNER_XYZ}, // right
			{CORNER_XYz, CORNER_xYz, CORNER_xYZ, CORNER_XYZ}, // top
			{CORNER_xyZ, CORNER_XyZ, CORNER_XYZ, CORNER_xYZ} // front
	};
	return corners[side];
}

const Vec3f & getNormal(const side_t side) {
	static const Vec3f normals[6] = {
			Vec3f(-1, 0, 0), // left
			Vec3f(0, -1, 0), // bottom
			Vec3f(0, 0, -1), // back
			Vec3f(1, 0, 0), // right
			Vec3f(0, 1, 0), // top
			Vec3f(0, 0, 1) // front
	};
	return normals[side];
}

std::vector<Box_f> splitUpBox(const Box_f & box, unsigned int partsX, unsigned int partsY, unsigned int partsZ) {
	if (box.isInvalid()) {
		std::cerr << __FILE__ << ":" << __LINE__ << ":" << std::endl
				  << "\t you are splitting invalid boxes" << std::endl;
	}

	std::vector<Box_f> result;
	const unsigned int num = partsX * partsY * partsZ;
	if (num == 0) {
		return result;
	} else if (num == 1) {
		result.emplace_back(box);
		return result;
	}
	result.reserve(num);

	float lastZ = box.getMinZ();
	float newMaxZ = 0.0f;
	for (unsigned int cz = 1; cz <= partsZ; ++cz) {
		float lastY = box.getMinY();
		float newMaxY = 0.0f;
		for (unsigned int cy = 1; cy <= partsY; ++cy) {
			float lastX = box.getMinX();
			float newMaxX;
			for (unsigned int cx = 1; cx <= partsX; ++cx) {
				// Interpolate between minimum and maximum values.
				if (cx == partsX) {
					newMaxX = box.getMaxX();
				} else {
					const float uX = static_cast<float>(partsX - cx) / static_cast<float>(partsX);
					const float vX = static_cast<float>(cx) / static_cast<float>(partsX);
					newMaxX = uX * box.getMinX() + vX * box.getMaxX();
				}
				if (cy == partsY) {
					newMaxY = box.getMaxY();
				} else {
					const float uY = static_cast<float>(partsY - cy) / static_cast<float>(partsY);
					const float vY = static_cast<float>(cy) / static_cast<float>(partsY);
					newMaxY = uY * box.getMinY() + vY * box.getMaxY();
				}
				if (cz == partsZ) {
					newMaxZ = box.getMaxZ();
				} else {
					const float uZ = static_cast<float>(partsZ - cz) / static_cast<float>(partsZ);
					const float vZ = static_cast<float>(cz) / static_cast<float>(partsZ);
					newMaxZ = uZ * box.getMinZ() + vZ * box.getMaxZ();
				}
				result.emplace_back(lastX, newMaxX, lastY, newMaxY, lastZ, newMaxZ);
				lastX = newMaxX;
			}
			lastY = newMaxY;
		}
		lastZ = newMaxZ;
	}
	return result;
}

std::vector<Box_f> splitBoxCubeLike(const Box_f & box) {
	static float isq2 = 1.0 / sqrt(2);

	float x = box.getExtentX(), y = box.getExtentY(), z = box.getExtentZ();

	if (x >= y) {
		if (y >= z) { // x >= y >= z
			if (x * isq2 > y)
				return splitUpBox(box, 2, 1, 1);
			else if (x * isq2 > z)
				return splitUpBox(box, 2, 2, 1);
			else
				return splitUpBox(box, 2, 2, 2);
		} else if (x >= z) { // x >= z >= y
			if (x * isq2 > z)
				return splitUpBox(box, 2, 1, 1);
			else if (x * isq2 > y)
				return splitUpBox(box, 2, 1, 2);
			else
				return splitUpBox(box, 2, 2, 2);
		} else { // z >= x >= y
			if (z * isq2 > x)
				return splitUpBox(box, 1, 1, 2);
			else if (z * isq2 > y)
				return splitUpBox(box, 2, 1, 2);
			else
				return splitUpBox(box, 2, 2, 2);
		}
	} else { // y >= x
		if (x >= z) { // y >= x >= z
			if (y * isq2 > x)
				return splitUpBox(box, 1, 2, 1);
			else if (y * isq2 > z)
				return splitUpBox(box, 2, 2, 1);
			else
				return splitUpBox(box, 2, 2, 2);
		} else if (y >= z) { // y >= z >= x
			if (y * isq2 > z)
				return splitUpBox(box, 1, 2, 1);
			else if (y * isq2 > x)
				return splitUpBox(box, 1, 2, 2);
			else
				return splitUpBox(box, 2, 2, 2);
		} else { // z >= y >= x
			if (z * isq2 > y)
				return splitUpBox(box, 1, 1, 2);
			else if (z * isq2 > x)
				return splitUpBox(box, 1, 2, 2);
			else
				return splitUpBox(box, 2, 2, 2);
		}
	}
}

Box_f getTransformedBox(const Box_f & box, const Matrix4x4f & matrix) {
	Box_f result;
	result.invalidate();
	result.include(matrix.transformPosition(box.getCorner(CORNER_xyz)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_Xyz)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_xYz)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_XYz)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_xyZ)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_XyZ)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_xYZ)));
	result.include(matrix.transformPosition(box.getCorner(CORNER_XYZ)));
	return result;
}
}
}
