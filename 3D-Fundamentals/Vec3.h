#pragma once
#include "Vec2.h"
template <typename T>
class _Vec3 : public _Vec2<T> {
public:
	_Vec3() = default;
	_Vec3(T x, T y, T z) : _Vec2<T>(x, y), z(z)
	{
	}
	_Vec3<T>& operator+=(const _Vec3<T>& v) {
		_Vec2<T>::x += v.x;
		_Vec2<T>::y += v.y;
		z += v.z;
		return *this;
	}
	T z;
};
typedef _Vec3<float> Vec3;
