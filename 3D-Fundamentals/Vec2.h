#pragma once
template <typename T>
class _Vec2
{
public:
	_Vec2() = default;
	_Vec2(T x, T y) : x(x), y(y)
	{
	}
public:
	T x;
	T y;
};

typedef _Vec2<float> Vec2;

