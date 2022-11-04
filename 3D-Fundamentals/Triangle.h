#pragma once
#include <vector>
#include "IndexedList.h"
#include "Vec3.h"
class Triangle {
public:
	Triangle(float size) : vertices(3)
	{
		vertices[0] = Vec3(-size, 0.0f, 0.0f);
		vertices[1] = Vec3(size, 0.0f, 0.0f);
		vertices[2] = Vec3(0.0f, size, 0.0f);
	}
	IndexedLineList GetLines() {
		return {
			vertices, {
				0, 1, 0, 2, 1, 2
}
		};
	}
private:
	std::vector<Vec3> vertices;

};