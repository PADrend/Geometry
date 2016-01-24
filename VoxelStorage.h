/*
	This file is part of the Geometry library.
	Copyright (C) 2014 Claudius Jähn <claudius@uni-paderborn.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef VOXEL_STORAGE_H
#define VOXEL_STORAGE_H

#include "Box.h"
#include "BoxIntersection.h"
#include "Vec3.h"
#include <array>
#include <cassert>
#include <stack>
#include <tuple>
#include <vector>
#include <memory>

namespace Geometry {

/*! The Voxelstorage is a spatial data structure for storing voxels(=arbitrary values) at integer positions.
	Internally, a dynamic octree is used as storage:
	 - the root node is adjusted automatically
	 - empty inner nodes are skipped
	 - subtrees having an uniform value are represented by single nodes

	\todo levels should only be skipped if the value is nullVoxel. Serialization should be easier then.
*/
template <typename Voxel_t, unsigned int blockSizePow = 2, typename integer_t = int32_t, typename uinteger_t = uint32_t>
class VoxelStorage {
	const Voxel_t nullVoxel;

public:
	static const uinteger_t blockSideLength = 1 << blockSizePow; // e.g. 1<<2 = 4
	static const uinteger_t blockMask = blockSideLength - 1; // e.g. 0b00000011
	static const uint32_t blockSize = blockSideLength * blockSideLength * blockSideLength;

	typedef _Vec3<integer_t> Vec3_t;
	typedef _Box<integer_t> Box_t;
	typedef std::array<Voxel_t, blockSize> block_t;

	typedef std::pair<std::vector<std::tuple<Vec3_t, uinteger_t, Voxel_t>>, // uniform areas
					  std::vector<std::tuple<Vec3_t, block_t>>> // blocks
			serializationData_t;

	// ------------
private:
	struct Area {
		const Vec3_t origin;
		const uinteger_t sideLength;

		union Data_t {
			std::array<Area *, 8> * children;
			block_t * block;
		} data;
		enum class DataType : uint8_t { CONTAINER, UNIFORM_AREA, BLOCK } dataType;
		bool markedForConsolidation;

		Voxel_t uniformValue;

		Area(const Vec3_t & _origin, uinteger_t _sideLength, const Voxel_t & _uniformValue)
				: origin(_origin),
				  sideLength(_sideLength),
				  dataType(DataType::UNIFORM_AREA),
				  uniformValue(_uniformValue) {
		}
		~Area() {
			clear();
		}
		void clear() {
			if (isContainer()) {
				delete data.children;
			} else if (isBlock()) {
				delete data.block;
			}
			dataType = DataType::UNIFORM_AREA;
		}
		bool contains(const Vec3_t & pos) const {
			return pos.x() >= origin.x() && pos.x() < origin.x() + static_cast<integer_t>(sideLength)
					&& pos.y() >= origin.y() && pos.y() < origin.y() + static_cast<integer_t>(sideLength)
					&& pos.z() >= origin.z() && pos.z() < origin.z() + static_cast<integer_t>(sideLength);
		}
		uint8_t getChildIndex(const Vec3_t & pos) const {
			const auto hSideLength = static_cast<integer_t>(sideLength) / 2;
			return (pos.x() >= origin.x() + hSideLength ? 1 : 0) + (pos.y() >= origin.y() + hSideLength ? 2 : 0)
					+ (pos.z() >= origin.z() + hSideLength ? 4 : 0);
		}
		Box_t getOctant(uint8_t i) const {
			const auto hSideLength = static_cast<integer_t>(sideLength) / 2;
			const Vec3_t minPos(origin.x() + ((i & 1) > 0 ? hSideLength : 0),
								origin.y() + ((i & 2) > 0 ? hSideLength : 0),
								origin.z() + ((i & 4) > 0 ? hSideLength : 0));
			return Box_t(minPos, minPos + Vec3_t(hSideLength - 1, hSideLength - 1, hSideLength - 1));
		}
		Vec3_t getOctantOrigin(uint8_t i) const {
			const auto hSideLength = static_cast<integer_t>(sideLength) / 2;
			return Vec3_t(origin.x() + ((i & 1) > 0 ? hSideLength : 0), origin.y() + ((i & 2) > 0 ? hSideLength : 0),
						  origin.z() + ((i & 4) > 0 ? hSideLength : 0));
		}
		bool isContainer() const {
			return dataType == DataType::CONTAINER;
		}
		bool isBlock() const {
			return dataType == DataType::BLOCK;
		}
		bool isUniform() const {
			return dataType == DataType::UNIFORM_AREA;
		}

		const Vec3_t & getOrigin() const {
			return origin;
		}
		Vec3_t getMaxPosition() const {
			return Vec3_t(origin.x() + sideLength - 1, origin.y() + sideLength - 1, origin.z() + sideLength - 1);
		}
		block_t * getBlock() const {
			return isBlock() ? data.block : nullptr;
		}
		Area * getChild(uint8_t i) const {
			return isContainer() ? data.children->at(i) : nullptr;
		}

		Box_t getBox() const {
			return Box_t(origin, origin + Vec3_t(sideLength - 1, sideLength - 1, sideLength - 1));
		}

		std::array<Area *, 8> & assureContainer() {
			if (!isContainer()) {
				assert(sideLength > blockSideLength);
				clear();
				data.children = new std::array<Area *, 8>;
				for (auto & c : *data.children)
					c = nullptr;
				dataType = DataType::CONTAINER;
			}
			return *data.children;
		}
		block_t & assureBlock() {
			if (!isBlock()) {
				assert(sideLength == blockSideLength);
				clear();
				data.block = new block_t;
				for (auto & v : *data.block)
					v = uniformValue;
				dataType = DataType::BLOCK;
			}
			return *data.block;
		}
		void convertToUniformArea(const Voxel_t & _uniformValue) {
			clear();
			uniformValue = _uniformValue;
			markedForConsolidation = false;
		}
		Area * insertIntermediateChild(const Vec3_t _origin, uinteger_t _sideLength) {
			assert(_sideLength < sideLength);
			assert(contains(_origin));
			auto & children = assureContainer();
			const uint8_t childIndex = getChildIndex(_origin);
			Area * newChild = new Area(_origin, _sideLength, uniformValue);
			Area * oldChild = children[childIndex];
			children[childIndex] = newChild;
			if (oldChild) {
				assert(oldChild->sideLength < _sideLength);
				auto & children2 = newChild->assureContainer();
				children2[newChild->getChildIndex(oldChild->origin)] = oldChild;
			}
			return newChild;
		}
		void setChild(uint8_t i, Area * child) {
			assureContainer();
			assert(data.children->at(i) == nullptr);
			data.children->at(i) = child;
		}
	};
	uint32_t posToBlockIdx(const Vec3_t & pos) const {
		return (pos.x() & blockMask) + (pos.y() & blockMask) * blockSideLength
				+ (pos.z() & blockMask) * blockSideLength * blockSideLength;
	}
	Vec3_t calcOrigin(const Vec3_t & pos, integer_t sideLength) const {
		const uint32_t areaMask = ~(sideLength - 1);
		return Vec3_t(pos.x() & areaMask, pos.y() & areaMask, pos.z() & areaMask);
	}

	std::unique_ptr<Area> root;

	std::pair<Vec3_t, integer_t> getEnclosingAreaBox(const Area & area, const Vec3_t & pos) const {
		Vec3_t newOrigin = area.getOrigin();
		uinteger_t newSideLength = area.sideLength;
		//			std::cout << "b: "<<newOrigin<<" : "<<newSideLength<<std::endl;
		while (!Box_t(newOrigin, newOrigin + Vec3_t(newSideLength - 1, newSideLength - 1, newSideLength - 1))
						.contains(pos)) {
			newSideLength *= 2;
			newOrigin = calcOrigin(area.getOrigin(), newSideLength);
			//				std::cout << "b: "<<newOrigin<<" : "<<newSideLength<<std::endl;
		}
		return std::make_pair(newOrigin, newSideLength);
	}

	std::pair<Vec3_t, integer_t> getEnclosingAreaBox(const Box_t & box) const {
		uinteger_t sideLength = blockSideLength;
		Vec3_t origin = calcOrigin(box.getMin(), sideLength);

		while (!Box_t(origin, origin + Vec3_t(sideLength - 1, sideLength - 1, sideLength - 1)).contains(box)) {
			sideLength *= 2;
			origin = calcOrigin(origin, sideLength);
		}
		return std::make_pair(origin, sideLength);
	}

	block_t & findOrCreateBlock(const Vec3_t & pos) {
		//			std::cout << "findOrCreateBlock("<<pos<<")"<<std::endl;
		if (!root) {
			root.reset(new Area(calcOrigin(pos, blockSideLength), blockSideLength, nullVoxel));
			//				std::cout << "creating root: "<<root->origin<<std::endl;
		}

		if (!root->contains(pos)) {
			const auto commonBox = getEnclosingAreaBox(*root.get(), pos);
			//				std::cout << "resetting root: "<<commonBox.first<<" : "<<commonBox.second<<std::endl;
			Area * newRoot = new Area(commonBox.first, commonBox.second, nullVoxel);
			Area * oldRoot = root.release();
			newRoot->setChild(newRoot->getChildIndex(oldRoot->getOrigin()), oldRoot);
			root.reset(newRoot);
		}

		Area * currentArea = root.get();

		while (true) {
			//				std::cout << " >("<<currentArea->origin<<" : "<<currentArea->sideLength<< ")"<<std::endl;
			currentArea->markedForConsolidation = true;
			if (currentArea->isBlock()) {
				return currentArea->assureBlock();
			} else if (currentArea->isContainer()) {
				Area * child = currentArea->getChild(currentArea->getChildIndex(pos));
				if (!child) {
					currentArea =
							currentArea->insertIntermediateChild(calcOrigin(pos, blockSideLength), blockSideLength);
				} else if (child->contains(pos)) {
					currentArea = child;
				} else { // insert intermediate area

					//						std::cout << "B) pos: "<<pos<< " box:"<<
					//currentArea->origin<<":"<<currentArea->sideLength <<" ";

					const auto commonBox = getEnclosingAreaBox(*child, pos);
					//						std::cout << "child:"<< child->origin<<":"<<child->sideLength <<" ";
					//						std::cout << "commonBox:"<< commonBox.first<<":"<<commonBox.second <<"
					//"<<std::endl;

					currentArea = currentArea->insertIntermediateChild(commonBox.first, commonBox.second);
				}
			} else { // if(currentArea->isUniform()){
				if (currentArea->sideLength == blockSideLength) {
					return currentArea->assureBlock();
				} else {
					currentArea =
							currentArea->insertIntermediateChild(calcOrigin(pos, blockSideLength), blockSideLength);
				}
			}
		}
	}
	void consolidate(Area * area) {
		if (area && area->markedForConsolidation) {
			area->markedForConsolidation = false;

			if (area->isBlock()) {
				const auto & block = area->assureBlock();
				Voxel_t value = block[0];
				for (uint32_t j = 1; j < blockSize; ++j) {
					if (block[j] != value)
						return; // simplification not possible
				}
				area->convertToUniformArea(value);
			} else if (area->isContainer()) {
				Voxel_t value = area->uniformValue;
				bool childrenAreUniform = true;

				for (uint8_t i = 0; i < 8; ++i) {
					Area * child = area->getChild(i);
					if (child && child->markedForConsolidation)
						consolidate(child);
					if (i == 0) { // first child
						if (child) {
							if (child->isUniform()) {
								value = child->uniformValue;
							} else {
								childrenAreUniform = false;
							}
						} // else keep area's uniform value
					} else if (child) {
						if (childrenAreUniform && (!child->isUniform() || child->uniformValue != value))
							childrenAreUniform = false;
					} else { // else !child
						if (childrenAreUniform && area->uniformValue != value)
							childrenAreUniform = false;
					}
				}
				if (childrenAreUniform) {
					area->convertToUniformArea(value);
					//						std::cout << "Consolidated area: "<<area->getBox()<<"\n";
				}
			} // else area->isUniform()
		}
	}

public:
	VoxelStorage(VoxelStorage && other) : nullVoxel(other.nullVoxel), root(std::move(other.root)) {
	}
	explicit VoxelStorage(const Voxel_t & _nullVoxel) : nullVoxel(_nullVoxel) {
	}

	//! Set the value @p voxel at the given @p position without consolidating (combining uniform subtrees)
	void _set(const Vec3_t & pos, const Voxel_t & voxel) {
		findOrCreateBlock(pos)[posToBlockIdx(pos)] = voxel;
	}

	//! Set the value @p voxel at the given @p position.
	void set(const Vec3_t & pos, const Voxel_t & voxel) {
		auto & block = findOrCreateBlock(pos);
		block[posToBlockIdx(pos)] = voxel;

		for (uint32_t i = 0; i < blockSize; ++i) { // check if block became uniform
			if (block[i] != voxel)
				return;
		}
		consolidate(root.get());
	}
	//! Return the value at the given @p position. If the value has not been set, nullVoxel is returned.
	const Voxel_t & get(const Vec3_t & pos) const {
		if (!root || !root->contains(pos))
			return nullVoxel;

		const Area * currentArea = root.get();
		while (true) {
			const block_t * block = currentArea->getBlock();
			if (block)
				return (*block)[posToBlockIdx(pos)];
			const Area * child = currentArea->getChild(currentArea->getChildIndex(pos));
			if (!child || !child->contains(pos))
				return currentArea->uniformValue;
			currentArea = child;
		}
	}
	//! Fill the given @p area with the given value @p voxel.
	void fill(const Box_t & fillArea, const Voxel_t & voxel) {
		//			std::cout << "fill:"<<fillArea<<" "<<std::endl;

		// assure properly sized root node.
		if (!root || !root->getBox().contains(fillArea)) {
			findOrCreateBlock(fillArea.getMin());
			findOrCreateBlock(fillArea.getMax());
		}

		//			std::cout << "start filling "<<std::endl;

		std::stack<Area *> todo;
		todo.push(root.get());
		while (!todo.empty()) {
			Area * currentArea = todo.top();
			todo.pop();
			//				std::cout << " > "<<currentArea->getBox()<<std::endl;

			const Box_t areaBox(currentArea->getBox());
			if (fillArea.contains(areaBox)) {
				currentArea->convertToUniformArea(voxel);
				//					std::cout << " >F "<< currentArea->origin<<" : "<<currentArea->sideLength
				//<<std::endl;
				//					std::cout << " >F "<< currentArea->sideLength;
			} else if (Intersection::isBoxIntersectingBox(areaBox, fillArea)) {
				currentArea->markedForConsolidation = true;
				if (currentArea->isBlock() || currentArea->sideLength == blockSideLength) {
					const Box_t intersection = Intersection::getBoxBoxIntersection(areaBox, fillArea);
					block_t & block = currentArea->assureBlock();
					for (integer_t x = intersection.getMinX(); x <= intersection.getMaxX(); ++x) {
						for (integer_t y = intersection.getMinY(); y <= intersection.getMaxY(); ++y) {
							for (integer_t z = intersection.getMinZ(); z <= intersection.getMaxZ(); ++z) {
								block[posToBlockIdx(Vec3_t(x, y, z))] = voxel;
							}
						}
					}
					//						std::cout << " >B "<< currentArea->origin<<" : "<<currentArea->sideLength
					//<<std::endl;
				} else { // uniform value or container
					currentArea->assureContainer();
					for (uint8_t i = 0; i < 8; ++i) {
						const Box_t octantIntersection =
								Intersection::getBoxBoxIntersection(currentArea->getOctant(i), fillArea);
						if (octantIntersection.isInvalid()) // no intersection?
							continue;
						Area * child = currentArea->getChild(i);
						if (child && child->getBox().contains(octantIntersection)) { // fill inside existing child...
							todo.push(child);
						} else if (!child) { // create new child covering the filling
							const auto enclosingBox = getEnclosingAreaBox(octantIntersection);
							//								std::cout << " >>a:              \t"<< currentArea->getOrigin()
							//<<std::endl;
							//								std::cout << " >>octant:"<<static_cast<int>(i)<<"         \t"<<
							//currentArea->getOctant(i) <<std::endl;
							//								std::cout << " >>octantIntersection:\t"<< octantIntersection
							//<<std::endl;
							//								std::cout << " >>enclosingBox:\t"<< enclosingBox.first<<" :
							//"<<enclosingBox.second <<std::endl;
							todo.push(currentArea->insertIntermediateChild(enclosingBox.first, enclosingBox.second));
						} else { // create an intermediate child covering the filled area and the old child
							//								std::cout << " B ";
							//								std::cout << "\n >>
							//octantIntersection:\t"<<octantIntersection;
							Box_t b = octantIntersection;
							b.include(child->getBox());
							//								std::cout << "\n >> childBox:\t"<<child->getBox();
							const auto enclosingBox = getEnclosingAreaBox(b);
							//								std::cout << "\n >> combined:\t"<<b;
							//								std::cout << "\n >> area:\t"<<enclosingBox.first<<" :
							//"<<enclosingBox.second<<"\n";
							todo.push(currentArea->insertIntermediateChild(enclosingBox.first, enclosingBox.second));
						}
					}
				}
				continue;
			} // else not intersecting -> skip
		}
		consolidate(root.get());
	}
	//		std::pair<bool,Voxel_t> isUniform(const Box_t& area)bool;
	//		bool isUniform(const Box_t& area,const Voxel_t& voxel)bool;
	//! Remove all values.
	void clear() {
		root.reset();
	}
	/*! experimental!
		\note queryBox must be block-aligned
		\todo Problem? ContainerArea( default=5, children= NULL,NULL,..., Uniform10) !?!?
	*/
	serializationData_t serialize(const Box_t & queryBox) const {
		return const_cast<VoxelStorage *>(this)->serialize(queryBox); // serialize should be const without this hack!
	}
	serializationData_t serialize(const Box_t & queryBox) {
		std::vector<std::tuple<Vec3_t, uinteger_t, Voxel_t>> uniformAreas;
		std::vector<std::tuple<Vec3_t, block_t>> singleValues;

		consolidate(root.get());
		std::stack<Area *> todo;
		if (root)
			todo.push(root.get());
		while (!todo.empty()) {
			Area * currentArea = todo.top();
			todo.pop();

			if (!Intersection::isBoxIntersectingBox(queryBox, currentArea->getBox()))
				continue;
			if (currentArea->isBlock()) {
				singleValues.emplace_back(currentArea->getOrigin(), currentArea->assureBlock());
			} else if (currentArea->isContainer()) {
				for (uint8_t i = 0; i < 8; ++i) {
					Area * child = currentArea->getChild(i);
					if (child)
						todo.push(child);
				}
			} else { // currentArea->isUniform
				if (currentArea->uniformValue != nullVoxel) // skip null
					uniformAreas.emplace_back(currentArea->getOrigin(), currentArea->sideLength,
											  currentArea->uniformValue);
			}
		}
		return std::make_pair(uniformAreas, singleValues);
	}

	void deserialize(const serializationData_t & data) {
		for (const auto & uniformArea : data.first) {
			const Box_t b(std::get<0>(uniformArea),
						  std::get<0>(uniformArea) + Vec3_t(std::get<1>(uniformArea), std::get<1>(uniformArea),
															std::get<1>(uniformArea)));
			fill(b, std::get<2>(uniformArea));
		}
		for (const auto & blockData : data.second)
			findOrCreateBlock(std::get<0>(blockData)) = std::get<1>(blockData);
		consolidate(root.get());
	}
	//! Get the (block aligned) bounding box around the set voxels.
	Box_t getBlockBounds() const {
		Box_t b;
		b.invalidate();
		std::stack<const Area *> todo;
		if (root)
			todo.push(root.get());
		while (!todo.empty()) {
			const Area * currentArea = todo.top();
			todo.pop();
			if (currentArea->isContainer()) {
				for (uint8_t i = 0; i < 8; ++i) {
					Area * child = currentArea->getChild(i);
					if (child && !b.contains(child->getBox()))
						todo.push(child);
				}
			} else if (currentArea->isBlock() || currentArea->uniformValue != nullVoxel) {
				b.include(currentArea->getBox());
			}
		}
		return b;
	}
};
}

#endif /* VOXEL_STORAGE_H */
