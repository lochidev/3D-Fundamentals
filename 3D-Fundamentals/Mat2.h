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
		const T cosTheta = cos(angle);
		const T sinTheta = sin(angle);
		v.x = cpy.x * cosTheta - cpy.y * sinTheta;
		v.y = cpy.x * sinTheta + cpy.y * cosTheta; // rsin1cos2 + rcos1sin2
	}
private:
	T x1;
	T x2;
	T y1;
	T y2;
};