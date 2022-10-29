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
		auto lines = c.GetLines();
		if (ip.IsPressed(G_B_PRESSED)) {
			angle = 0;
		}
		for (Vec3<float>& v : lines.vertices) {
			angle += ip.IsPressed(G_LEFT_PRESSED) ? (M_PI / 170) : ip.IsPressed(G_RIGHT_PRESSED) ? -(M_PI / 170) : 0;
			Mat2<float>::RotateZ(v, WrapAngle((angle))); // roll first
			//Mat2<float>::RotateX(v, WrapAngle(((M_PI / 6)))); // then pitch
			//Mat2<float>::RotateY(v, WrapAngle(((M_PI / 6)))); // then yaw
			depth += ip.IsPressed(G_UP_PRESSED) ? 0.05f : ip.IsPressed(G_DOWN_PRESSED) ? -.05f : 0.0f;
			v += {0.0f, 0.0f, depth};
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

