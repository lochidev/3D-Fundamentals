#pragma once
#include "Vec3.h"
#include <cmath>
template <typename T>
class Mat2 {
public:
	Mat2() = default;
	Mat2(T x1, T x2, T y1, T y2) : x1(x1), x2(x2), y1(y1), y2(y2){}
	static void Rotate(Vec3<T>& v, float angle) {
		Vec3<T> cpy(v);
		v.x = cpy.x * cos(angle) - cpy.y * sin(angle);
		v.y = cpy.x * sin(angle) + cpy.y * cos(angle); // rsin1cos2 + rcos1sin2
	}
private:
	T x1;
	T x2;
	T y1;
	T y2;
};