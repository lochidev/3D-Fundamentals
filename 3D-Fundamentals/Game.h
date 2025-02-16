#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
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
#include "Scenes/MainScene.h"
#include "Scenes/LUTScene.h"
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
					if (TTF_Init() == -1) {
						SDL_Log("SDL_ttf initialization failed: %s", TTF_GetError());
						return;
					}
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
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(const Game&&) = delete;
	Game& operator=(const Game&&) = delete;
	void Setup() {
		scenes.push_back(std::make_unique<LUTScene>(renderer, transformer, ip, Width, Height));
		currentScene = scenes.begin()
	}
	void Update() {
		ip.HandleEvents();
		(*currentScene)->Update();
	}
	void Render() {
		(*currentScene)->Render();
	}
	void Stop() {
		ip.isRunning = false;
	}
	bool IsRunning() {
		return ip.isRunning;
	}
	
public:
	const int Height;
	const int Width;
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	NDCTransformer<float> transformer;
	SDL_GameController* gController = nullptr;
	std::vector<std::unique_ptr<Scene<float>>> scenes;
	std::vector<std::unique_ptr<Scene<float>>>::iterator currentScene;
	InputManager ip;
};

