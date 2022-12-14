#pragma once
#include "Vec2.h"
#include "Vec3.h"
template <typename T>
class NDCTransformer {
public:
	NDCTransformer(int sw, int sh) : xFactor(sw / 2), yFactor(sh / 2)
	{
	}
	void TransformNDC(_Vec2<T> v) const {
		v.x = (1.0f + v.x) * xFactor;
		v.y = (1.0f - v.y) * yFactor;
	}
	void TransformNDC(_Vec3<T>& v) const {
			const T inverse = 1.0f / v.z;
			v.x = (1.0f + (v.x * inverse)) * xFactor;
			v.y = (1.0f - (v.y * inverse)) * yFactor;
	}
private:
	int xFactor;
	int yFactor;
};