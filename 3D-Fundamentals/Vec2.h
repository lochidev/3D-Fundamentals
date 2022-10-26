#pragma once
template <typename T>
class Vec2
{
public:
	Vec2() = default;
	Vec2(T x, T y) : x(x), y(y)
	{
	}
public:
	T x;
	T y;
};

