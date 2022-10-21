#pragma once
#include <vector>
#include "Vec3.h"
#include "IndexedLineList.h"
class Cube {
public:
	Cube(float size)
	{
		const float side = size / 2.00f;
		vertices.emplace_back(side, side, side);
		vertices.emplace_back(-side, side, side);
		vertices.emplace_back(side, -side, side);
		vertices.emplace_back(side, side, -side);
		vertices.emplace_back(-side, -side, -side);
		vertices.emplace_back(side, -side, -side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(-side, -side, side);
	}
	IndexedLineList GetLines() {
		return {
			vertices,{
				0,1, 0,2, 0,3, 4,5,
				4,6, 4,7, 1,6, 3,6,
				3,5, 1,7, 2,5, 7,2
			}

		};
	}
private:
	std::vector<Vec3<float>> vertices;
};