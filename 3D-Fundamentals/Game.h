#pragma once
#include "SDL.h"
#include "Vec2.h"
#include "Vec3.h"
#include "NDCTransformer.h"
#include "Triangle.h"
#include "Mat2.h"
#include <cmath>
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
					isRunning = true;
				}
			}
		}
		else {
			isRunning = false;
		}
	}
	void Clean() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	void Stop() {
		isRunning = false;
	}
	void HandleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
		default:
			break;
		}
	}
	void Update() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		Triangle t(0.5f);
		auto ticks = SDL_GetTicks();
		auto lines = t.GetLines();
		for (Vec3<float>& v : lines.vertices) {
			Mat2<float>::Rotate(v, WrapAngle(((M_PI / 6) * ticks)));
			transformer.TransformNDC(v);
		}
		for (auto i = lines.indexes.cbegin(),
			end = lines.indexes.cend();
			i != end; std::advance(i, 2)) {
			Vec3<float>& v1 = lines.vertices[*i];
			Vec3<float>& v2 = lines.vertices[*std::next(i)];
			SDL_RenderDrawLine(renderer, int(v1.x), int(v1.y), int(v2.x), int(v2.y));
		}


	}
	void Render() {
		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
	template <typename T>
	T WrapAngle(T angle) {
		const T modulo = fmod(angle,((T)2 * (T)M_PI));
		return (modulo > (T)M_PI) ? modulo - (T)2 * (T)M_PI : modulo;
	}
public:
	const int Height;
	const int Width;
	bool isRunning = false;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	NDCTransformer<float> transformer;
};

