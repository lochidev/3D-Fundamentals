#pragma once
#include "SDL.h"
#include "Vec2.h"
#include "Vec3.h"
#include "NDCTransformer.h"
#include "Triangle.h"
#include "Cube.h"
#include "Mat2.h"
#include <cmath>
#include "SDL_gamecontroller.h"
#include "InputManager.h"
#include <utility>
class Game
{
public:
	Game(const char* title, int x, int y, bool fullscreen) : Height(y), Width(x), transformer(x, y)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
			window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
			if (window) {
				renderer = SDL_CreateRenderer(window, -1, 0);
				if (renderer) {
					ip.isRunning = true;
					gController = SDL_GameControllerOpen(0);
					for (size_t i = 0; i < SDL_NumJoysticks(); i++)
					{
						if (SDL_IsGameController(i)) {
							gController = SDL_GameControllerOpen(i);
						}
					}
					ip.gController = gController;
					return;
				}
			}
		}
		ip.isRunning = false;
	}
	~Game() {
		if (gController != NULL) {
			SDL_GameControllerClose(gController);
		}
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(const Game&&) = delete;
	Game& operator=(const Game&&) = delete;
	void Stop() {
		ip.isRunning = false;
	}
	void Update() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		Cube c(3.0f);
		auto ticks = SDL_GetTicks();
		auto lines = c.GetTriangles();
		if (ip.IsPressed(G_B_PRESSED)) {
			angle = 0;
		}
		for (Vec3& v : lines.vertices) {
			angle += ip.IsPressed(G_LEFT_PRESSED) ? (M_PI / 170)
				: ip.IsPressed(G_RIGHT_PRESSED) ? -(M_PI / 170) : 0;
			if (ip.IsPressed(G_A_PRESSED))
				Mat2<float>::RotateZ(v, WrapAngle((angle))); // roll first
			if (ip.IsPressed(G_X_PRESSED))
				Mat2<float>::RotateX(v, WrapAngle((angle))); // then pitch
			if (ip.IsPressed(G_Y_PRESSED))
				Mat2<float>::RotateY(v, WrapAngle((angle))); // then yaw
			depth += ip.IsPressed(G_UP_PRESSED) ? 0.05f
				: ip.IsPressed(G_DOWN_PRESSED) ? -.05f : 0.0f;
			v += {0.0f, 0.0f, depth};
			transformer.TransformNDC(v);
		}
		for (auto i = lines.indexes.cbegin(),
			end = lines.indexes.cend();
			i != end; std::advance(i, 3)) {
			Vec3& v1 = lines.vertices[*i];
			Vec3& v2 = lines.vertices[*(std::next(i))];
			Vec3& v3 = lines.vertices[*(std::next(i, 2))];
			DrawTriangle(v1, v2, v3);
			//SDL_RenderPresent(renderer);
		}
		//DrawTriangle({ 0, 500 }, { 300, 300 }, { 1280, 400 });

	}
	void Render() {
		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
private:
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
	void DrawFlatTopTriangle(const Vec2& l, const Vec2& r, const Vec2& b)
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

	void DrawFlatBottomTriangle(const Vec2& l, const  Vec2& r, const Vec2& h)
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
	T WrapAngle(T angle) {
		const T modulo = fmod(angle, ((T)2 * (T)M_PI));
		return (modulo > (T)M_PI) ? modulo - (T)2 * (T)M_PI : modulo;
	}
public:
	const int Height;
	const int Width;
	InputManager ip;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	NDCTransformer<float> transformer;
	SDL_GameController* gController = nullptr;
	double angle = 0;
	float depth = 6.0f;
};

