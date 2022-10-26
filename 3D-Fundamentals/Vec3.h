#pragma once
#include "Vec2.h"
template <typename T>
class Vec3 : public Vec2<T> {
public:
	Vec3() = default;
	Vec3(T x, T y, T z) : Vec2<T>(x, y), z(z)
	{
	}
	Vec3<T>& operator+=(const Vec3<T>& v) {
		Vec2<T>::x += v.x;
		Vec2<T>::y += v.y;
		z += v.z;
		return *this;
	}
	T z;
};