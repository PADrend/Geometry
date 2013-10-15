/*
	This file is part of the Geometry library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "RayBoxIntersection.h"
#include "Box.h"
#include "Line.h"

namespace Geometry {
namespace Intersection {

template<typename value_t>
Slope<value_t>::Slope(const ray_t & p_ray) :
	ray(p_ray),
	inverseDirection(1.0 / p_ray.getDirection().getX(),
					 1.0 / p_ray.getDirection().getY(),
					 1.0 / p_ray.getDirection().getZ()) {
	const auto & ori = p_ray.getOrigin();
	const auto & dir = p_ray.getDirection();
	// Ray slope
	xByY = dir.getX() * inverseDirection.getY();
	xByZ = dir.getX() * inverseDirection.getZ();
	yByX = dir.getY() * inverseDirection.getX();
	yByZ = dir.getY() * inverseDirection.getZ();
	zByY = dir.getZ() * inverseDirection.getY();
	zByX = dir.getZ() * inverseDirection.getX();
	c_xy = ori.getY() - yByX * ori.getX();
	c_xz = ori.getZ() - zByX * ori.getX();
	c_yx = ori.getX() - xByY * ori.getY();
	c_yz = ori.getZ() - zByY * ori.getY();
	c_zx = ori.getX() - xByZ * ori.getZ();
	c_zy = ori.getY() - yByZ * ori.getZ();

	// Ray slope classification
	if(dir.getX() < 0) {
		if(dir.getY() < 0) {
			if(dir.getZ() < 0) {
				classification = MMM;
			} else if(dir.getZ() > 0) {
				classification = MMP;
			} else { //(dir.getZ() == 0)
				classification = MMO;
			}
		} else { //(dir.getY() >= 0)
			if(dir.getZ() < 0) {
				classification = MPM;
				if(dir.getY() == 0) {
					classification = MOM;
				}
			} else { //(dir.getZ() >= 0)
				if((dir.getY() == 0) && (dir.getZ() == 0)) {
					classification = MOO;
				} else if(dir.getZ() == 0) {
					classification = MPO;
				} else if(dir.getY() == 0) {
					classification = MOP;
				} else {
					classification = MPP;
				}
			}
		}
	} else { //(dir.getX() >= 0)
		if(dir.getY() < 0) {
			if(dir.getZ() < 0) {
				classification = PMM;
				if(dir.getX() == 0) {
					classification = OMM;
				}
			} else { //(dir.getZ() >= 0)
				if((dir.getX() == 0) && (dir.getZ() == 0)) {
					classification = OMO;
				} else if(dir.getZ() == 0) {
					classification = PMO;
				} else if(dir.getX() == 0) {
					classification = OMP;
				} else {
					classification = PMP;
				}
			}
		} else { //(dir.getY() >= 0)
			if(dir.getZ() < 0) {
				if((dir.getX() == 0) && (dir.getY() == 0)) {
					classification = OOM;
				} else if(dir.getX() == 0) {
					classification = OPM;
				} else if(dir.getY() == 0) {
					classification = POM;
				} else {
					classification = PPM;
				}
			} else { //(dir.getZ() > 0)
				if(dir.getX() == 0) {
					if(dir.getY() == 0) {
						classification = OOP;
					} else if(dir.getZ() == 0) {
						classification = OPO;
					} else {
						classification = OPP;
					}
				} else {
					if((dir.getY() == 0) && (dir.getZ() == 0)) {
						classification = POO;
					} else if(dir.getY() == 0) {
						classification = POP;
					} else if(dir.getZ() == 0) {
						classification = PPO;
					} else {
						classification = PPP;
					}
				}
			}
		}
	}
}

template<typename value_t>
bool Slope<value_t>::isRayIntersectingBox(const box_t & box) const {
	const auto & ori = ray.getOrigin();
	switch(classification) {
		case MMM:
			return !((ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
						(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMaxX() + c_yx > 0) ||
						(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0) ||
						(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0));
		case MMP:
			return !((ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMaxX() + c_yx > 0) ||
						(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMinZ() + c_yz < 0) ||
						(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0));
		case MPM:
			return !((ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
						(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0) ||
						(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0) ||
						(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0));
		case MPP:
			return !((ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0) ||
						(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0) ||
						(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0));
		case PMM:
			return !((ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
						(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMinX() + c_yx < 0) ||
						(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0) ||
						(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0));
		case PMP:
			return !((ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMinX() + c_yx < 0) ||
						(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMinZ() + c_yz < 0) ||
						(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0));
		case PPM:
			return !((ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
						(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMinX() + c_yx < 0) ||
						(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0) ||
						(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0));
		case PPP:
			return !((ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMinX() + c_yx < 0) ||
						(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0) ||
						(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0));
		case OMM:
			return !((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
						(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0));
		case OMP:
			return !((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
						(zByY * box.getMinY() - box.getMinZ() + c_yz < 0));
		case OPM:
			return !((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
						(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0));
		case OPP:
			return !((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
						(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
						(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0));
		case MOM:
			return !((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getX() < box.getMinX()) || (ori.getZ() < box.getMinZ()) ||
						(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0));
		case MOP:
			return !((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getX() < box.getMinX()) || (ori.getZ() > box.getMaxZ()) ||
						(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0));
		case POM:
			return !((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getX() > box.getMaxX()) || (ori.getZ() < box.getMinZ()) ||
						(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
						(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0));
		case POP:
			return !((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getX() > box.getMaxX()) || (ori.getZ() > box.getMaxZ()) ||
						(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
						(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0));
		case MMO:
			return !((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
						(ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) ||
						(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMaxX() + c_yx > 0));
		case MPO:
			return !((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
						(ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) ||
						(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0));
		case PMO:
			return !((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
						(ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) ||
						(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
						(xByY * box.getMinY() - box.getMinX() + c_yx < 0));
		case PPO:
			return !((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
						(ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) ||
						(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
						(xByY * box.getMaxY() - box.getMinX() + c_yx < 0));

		case MOO:
			return !((ori.getX() < box.getMinX()) ||
						(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()));

		case POO:
			return !((ori.getX() > box.getMaxX()) ||
						(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
						(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()));
		case OMO:
			return !((ori.getY() < box.getMinY()) ||
						(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()));
		case OPO:
			return !((ori.getY() > box.getMaxY()) ||
						(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()));
		case OOM:
			return !((ori.getZ() < box.getMinZ()) ||
						(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()));
		case OOP:
			return !((ori.getZ() > box.getMaxZ()) ||
						(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
						(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()));
		default:
			return false;
	}
	return false;
}

template<typename value_t>
bool Slope<value_t>::getRayBoxIntersection(const box_t & box,
										   value_t & intersection) const {
	const auto & ori = ray.getOrigin();
	switch(classification) {
		case MMM: {
			if((ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
				(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMaxX() + c_yx > 0) ||
				(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0) ||
				(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MMP: {
			if((ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMaxX() + c_yx > 0) ||
				(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMinZ() + c_yz < 0) ||
				(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MPM: {
			if((ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
				(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0) ||
				(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0) ||
				(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MPP: {
			if((ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0) ||
				(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0) ||
				(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case PMM: {
			if((ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
				(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMinX() + c_yx < 0) ||
				(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0) ||
				(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case PMP: {
			if((ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMinX() + c_yx < 0) ||
				(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMinZ() + c_yz < 0) ||
				(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case PPM: {
			if((ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
				(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMinX() + c_yx < 0) ||
				(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0) ||
				(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case PPP: {
			if((ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMinX() + c_yx < 0) ||
				(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0) ||
				(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case OMM: {
			if((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() < box.getMinY()) || (ori.getZ() < box.getMinZ()) ||
				(yByZ * box.getMinZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMaxZ() + c_yz > 0)) {
				return false;
			}
			intersection = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case OMP: {
			if((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() < box.getMinY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByZ * box.getMaxZ() - box.getMaxY() + c_zy > 0) ||
				(zByY * box.getMinY() - box.getMinZ() + c_yz < 0)) {
				return false;
			}
			intersection = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case OPM: {
			if((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() > box.getMaxY()) || (ori.getZ() < box.getMinZ()) ||
				(yByZ * box.getMinZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMaxZ() + c_yz > 0)) {
				return false;
			}
			intersection = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case OPP: {
			if((ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() > box.getMaxY()) || (ori.getZ() > box.getMaxZ()) ||
				(yByZ * box.getMaxZ() - box.getMinY() + c_zy < 0) ||
				(zByY * box.getMaxY() - box.getMinZ() + c_yz < 0)) {
				return false;
			}
			intersection = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MOM: {
			if((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getX() < box.getMinX()) || (ori.getZ() < box.getMinZ()) ||
				(zByX * box.getMinX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MOP: {
			if((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getX() < box.getMinX()) || (ori.getZ() > box.getMaxZ()) ||
				(zByX * box.getMinX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMaxX() + c_zx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case POM: {
			if((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getX() > box.getMaxX()) || (ori.getZ() < box.getMinZ()) ||
				(zByX * box.getMaxX() - box.getMaxZ() + c_xz > 0) ||
				(xByZ * box.getMinZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t2 = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case POP: {
			if((ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getX() > box.getMaxX()) || (ori.getZ() > box.getMaxZ()) ||
				(zByX * box.getMaxX() - box.getMinZ() + c_xz < 0) ||
				(xByZ * box.getMaxZ() - box.getMinX() + c_zx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t2 = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			if(t2 > intersection) {
				intersection = t2;
			}
			return true;
		}
		case MMO: {
			if((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
				(ori.getX() < box.getMinX()) || (ori.getY() < box.getMinY()) ||
				(yByX * box.getMinX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMaxX() + c_yx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			return true;
		}
		case MPO: {
			if((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
				(ori.getX() < box.getMinX()) || (ori.getY() > box.getMaxY()) ||
				(yByX * box.getMinX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMaxX() + c_yx > 0)) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			return true;
		}
		case PMO: {
			if((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
				(ori.getX() > box.getMaxX()) || (ori.getY() < box.getMinY()) ||
				(yByX * box.getMaxX() - box.getMaxY() + c_xy > 0) ||
				(xByY * box.getMinY() - box.getMinX() + c_yx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			return true;
		}
		case PPO: {
			if((ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ()) ||
				(ori.getX() > box.getMaxX()) || (ori.getY() > box.getMaxY()) ||
				(yByX * box.getMaxX() - box.getMinY() + c_xy < 0) ||
				(xByY * box.getMaxY() - box.getMinX() + c_yx < 0)) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			const value_t t1 = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			if(t1 > intersection) {
				intersection = t1;
			}
			return true;
		}
		case MOO:
			if((ori.getX() < box.getMinX()) ||
				(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ())) {
				return false;
			}
			intersection = (box.getMaxX() - ori.getX()) * inverseDirection.getX();
			return true;
		case POO:
			if((ori.getX() > box.getMaxX()) ||
				(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY()) ||
				(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ())) {
				return false;
			}
			intersection = (box.getMinX() - ori.getX()) * inverseDirection.getX();
			return true;
		case OMO:
			if((ori.getY() < box.getMinY()) ||
				(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ())) {
				return false;
			}
			intersection = (box.getMaxY() - ori.getY()) * inverseDirection.getY();
			return true;
		case OPO:
			if((ori.getY() > box.getMaxY()) ||
				(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getZ() < box.getMinZ()) || (ori.getZ() > box.getMaxZ())) {
				return false;
			}
			intersection = (box.getMinY() - ori.getY()) * inverseDirection.getY();
			return true;
		case OOM:
			if((ori.getZ() < box.getMinZ()) ||
				(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY())) {
				return false;
			}
			intersection = (box.getMaxZ() - ori.getZ()) * inverseDirection.getZ();
			return true;
		case OOP:
			if((ori.getZ() > box.getMaxZ()) ||
				(ori.getX() < box.getMinX()) || (ori.getX() > box.getMaxX()) ||
				(ori.getY() < box.getMinY()) || (ori.getY() > box.getMaxY())) {
				return false;
			}
			intersection = (box.getMinZ() - ori.getZ()) * inverseDirection.getZ();
			return true;
		default:
			return false;
	}
	return false;
}

// Instantiate the template with float and double
template class Slope<float>;
template class Slope<double>;

}
}
