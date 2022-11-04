#pragma once
#include "Vec3.h"
#include <vector>
struct IndexedList {
	std::vector<Vec3> vertices;
	std::vector<size_t> indexes;
};
typedef IndexedList IndexedLineList;
typedef IndexedList IndexedTriangleList;