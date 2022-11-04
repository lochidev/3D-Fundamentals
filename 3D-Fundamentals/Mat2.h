#pragma once
#include "Vec3.h"
#include <cmath>
template <typename T>
class Mat2 {
public:
	Mat2() = default;
	Mat2(T x1, T x2, T y1, T y2) : x1(x1), x2(x2), y1(y1), y2(y2){}
	static void RotateZ(_Vec3<T>& v, float angle) {
		_Vec3<T> cpy(v);
		const T cosTheta = cos(angle);
		const T sinTheta = sin(angle);
		v.x = cpy.x * cosTheta - cpy.y * sinTheta;
		v.y = cpy.x * sinTheta + cpy.y * cosTheta; // rsin1cos2 + rcos1sin2
	}
	static void RotateX(_Vec3<T>& v, float angle) {
		_Vec3<T> cpy(v);
		const T cosTheta = cos(angle);
		const T sinTheta = sin(angle);
		v.y = cpy.y * cosTheta - cpy.z * sinTheta;
		v.z = cpy.y * sinTheta + cpy.z * cosTheta; 
	}
	static void RotateY(_Vec3<T>& v, float angle) {
		_Vec3<T> cpy(v);
		const T cosTheta = cos(angle);
		const T sinTheta = sin(angle);
		v.z = cpy.z * cosTheta - cpy.x * sinTheta;
		v.x = cpy.x * cosTheta + cpy.z * sinTheta; 
	}
private:
	T x1;
	T x2;
	T y1;
	T y2;
};