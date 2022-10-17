#pragma once
#include "Vec2.h"
template <typename T>
class Vec3 : public Vec2<T> {
public:
	Vec3() = default;
	Vec3(T x, T y, T z) : Vec2<T>(x, y), z(z)
	{
	}
	T z;
};