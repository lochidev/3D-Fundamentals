#pragma once
#include "SDL.h"
#include "Vec2.h"
#include "Vec3.h"
#include "NDCTransformer.h"
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
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//SDL_RenderClear(renderer);
		Vec3<float> v(0.25f, 0.25f, 0.0f);
		transformer.TransformNDC(v);
		SDL_RenderDrawPoint(renderer, int(v.x), int(v.y));
	}
	void Render() {
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
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

