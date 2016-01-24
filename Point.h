/*
	This file is part of the Geometry library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H

namespace Geometry {

/**
 * Capsule for an immutable position.
 *
 * @author Benjamin Eikel, Claudius Jähn
 * @date 2011-12-02
 */
template <typename Vector_t>
class Point {
private:
	const Vector_t position;

public:
	typedef Vector_t vector_t;

	explicit Point(const Vector_t & pos) : position(pos) {
	}

	bool operator==(const Point & other) const {
		return position == other.position;
	}

	const Vector_t & getPosition() const {
		return position;
	}
};
}

#endif /* GEOMETRY_POINT_H */
