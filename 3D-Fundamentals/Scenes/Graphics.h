#pragma once
#include <cmath>
#include "Vec3.h"
class Graphics {

public:
	static void DrawTriangle(const Vec2& l, const Vec2& r, const Vec2& h, SDL_Renderer* renderer) {
		// find highest in y axis
		const Vec2* p0 = &h;	//highest
		const Vec2* p1 = &l; //left
		const Vec2* p2 = &r; //right

		//sorting by y
		if (p2->y < p1->y)
			std::swap(p2, p1);
		if (p1->y < p0->y) {
			std::swap(p1, p0);
			if (p2->y < p1->y)
				std::swap(p2, p1);
		}
		// check if highest (p0) is equal to any other
		if (p0->y == p1->y) {
			if (p1->x > p0->x) std::swap(p1, p0);
			DrawFlatTopTriangle(*p1, *p0, *p2, renderer);
		}
		else if (p1->y == p2->y) {
			if (p1->x > p2->x) std::swap(p1, p2);
			DrawFlatBottomTriangle(*p1, *p2, *p0, renderer);
			//SDL_RenderPresent(renderer);

		}
		else {
			// divide
			const float slope = (p2->x - p0->x) / (p2->y - p0->y);
			const auto y = p1->y - p0->y;
			const auto x = y * slope;
			Vec2 tmp = { x + p0->x, p1->y };
			const Vec2* p3 = &tmp;
			if (p1->x > p3->x) std::swap(p1, p3);
			DrawFlatTopTriangle(*p1, *p3, *p2, renderer);
			//SDL_RenderPresent(renderer);

			DrawFlatBottomTriangle(*p1, *p3, *p0, renderer);
			//SDL_RenderPresent(renderer);

		}
		//SDL_RenderPresent(renderer);
	}
	static void DrawFlatTopTriangle(const Vec2& l, const Vec2& r, const Vec2& b, SDL_Renderer* renderer)
	{
		const float slopel = (b.x - l.x) / (b.y - l.y);
		const float sloper = (b.x - r.x) / (b.y - r.y);
		const auto yStart = (int)std::ceil(l.y - 0.5f);
		const auto yEnd = (int)std::ceil(b.y - 0.5f);

		for (size_t i = yStart; i < yEnd; i++)
		{
			const auto y = (i + 0.5f - l.y);
			const auto lx = slopel * y;
			const auto xStart = (int)std::ceil(lx + l.x - 0.5f);
			const auto rx = sloper * y;
			const auto xEnd = (int)std::ceil(rx + r.x - 0.5f);
			SDL_RenderDrawLine(renderer, xStart, i, xEnd - 1, i);
		}

	}

	static void DrawFlatBottomTriangle(const Vec2& l, const  Vec2& r, const Vec2& h, SDL_Renderer* renderer)
	{
		const float slopel = (h.x - l.x) / (h.y - l.y);
		const float sloper = (h.x - r.x) / (h.y - r.y);
		const auto yEnd = (int)std::ceil(l.y - 0.5f);
		const auto yStart = (int)std::ceil(h.y - 0.5f);

		for (size_t i = yStart; i < yEnd; i++)
		{
			const auto y = (i + 0.5f - h.y);
			const auto lx = slopel * y;
			const auto xStart = (int)std::ceil(lx + h.x - 0.5f);
			const auto rx = sloper * y;
			const auto xEnd = (int)std::ceil(rx + h.x - 0.5f);
			SDL_RenderDrawLine(renderer, xStart, i, xEnd - 1, i);
			//SDL_RenderPresent(renderer);
		}

	}
	template <typename T>
	static T WrapAngle(T angle) {
		const T modulo = fmod(angle, ((T)2 * (T)M_PI));
		return (modulo > (T)M_PI) ? modulo - (T)2 * (T)M_PI : modulo;
	}
};