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
			const T inverse = 1.0f / v.z;
			v.x = xFactor + (v.x * inverse) * xFactor;
			v.y = yFactor - (v.y * inverse) * yFactor;
			//if (v.x > xFactor * 2) {
			//	v.x = v.x > (xFactor * 2) ? (xFactor * 2) : 0;
			//	v.y = v.y > (yFactor * 2) ? (yFactor * 2) : 0;

			//}
	}
private:
	int xFactor;
	int yFactor;
};