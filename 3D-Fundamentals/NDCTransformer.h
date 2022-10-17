#pragma once
#include "Vec2.h"
#include "Vec3.h"
template <typename T>
class NDCTransformer {
public:
	NDCTransformer(int sw, int sh) : xFactor(sw / 2), yFactor(sh / 2)
	{
	}
	void TransformNDC(Vec2<T>& v) {
		v.x = xFactor + v.x * xFactor;
		v.y = yFactor - v.y * yFactor;
	}
	void TransformNDC(Vec3<T>& v) {
		TransformNDC((Vec2<T>&)v);
	}
private:
	int xFactor;
	int yFactor;
};