#pragma once
#include <vector>
struct IndexedLineList {
	std::vector<Vec3<float>> vertices;
	std::vector<size_t> indexes;
};