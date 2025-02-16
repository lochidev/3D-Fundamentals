#pragma once
#include <cmath>
#include "Vec3.h"
#include "ZBuffer.h"
class Graphics {

public:
	Graphics(SDL_Renderer* renderer, std::shared_ptr<ZBuffer> zBuf)
		:
		renderer(renderer),
		zBuf(zBuf)
	{}
	void DrawTriangle(const Vec2& l, const Vec2& r, const Vec2& h) {
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
			DrawFlatTopTriangle(*p1, *p0, *p2);
		}
		else if (p1->y == p2->y) {
			if (p1->x > p2->x) std::swap(p1, p2);
			DrawFlatBottomTriangle(*p1, *p2, *p0);
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
			DrawFlatTopTriangle(*p1, *p3, *p2);
			//SDL_RenderPresent(renderer);

			DrawFlatBottomTriangle(*p1, *p3, *p0);
			//SDL_RenderPresent(renderer);

		}
		//SDL_RenderPresent(renderer);
	}
	void DrawFlatTopTriangle(const Vec3& l, const Vec3& r, const Vec2& b)
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

	void DrawFlatBottomTriangle(const Vec3& l, const  Vec3& r, const Vec2& h)
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
			DrawLine(xStart, i, xEnd - 1, i);
			//SDL_RenderPresent(renderer);
		}

	}
	void DrawLine(Vec3 v1, Vec3 v2)
	{
		// copied from another project of mine, defined for compatability
		const float y1 = v1.y;
		const float y2 = v2.y;
		const float x1 = v1.x;
		const float x2 = v2.x;
		// y = mx + c (c is useless)
		// m = y2-y1/x2-x1
		const float dy = y2 - y1;
		const float dx = x2 - x1;
		const float slope = dy / dx;
		if (dy == 0.0f && dx == 0.0f) {
			if (zBuf->TestAndSet(x1, y1, v1.z)) {
				SDL_RenderDrawPoint(renderer, int(x1), int(y1));
			}
		}
		else if (dx > 0.0f) {
			float y = y1;
			for (int x = (int)x1; x < (x1 + dx); x++)
			{
				y += slope;
				if (zBuf->TestAndSet(x, y, v1.z)) {
					SDL_RenderDrawPoint(renderer, x, int(y));
				}
			}
		}
		else if (dx < 0.0f) {
			float y = y2;
			for (int x = (int)x2; x < (x2 + abs(dx)); x++)
			{
				y += slope;
				if (zBuf->TestAndSet(x, y, v1.z)) {

					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}
	}
	void DrawCircle(const Vec2& center, int radius)
	{
		for (double i = 0; i < 360; i += 0.01f)
		{
			SDL_RenderDrawPoint(renderer, center.x + radius * cos(i), center.y + radius * sin(i));
		}

	}
	template <typename T>
	static T WrapAngle(T angle) {
		const T modulo = fmod(angle, ((T)2 * (T)M_PI));
		return (modulo > (T)M_PI) ? modulo - (T)2 * (T)M_PI : modulo;
	}
private:
	SDL_Renderer* renderer;
	std::shared_ptr<ZBuffer> zBuf;
};