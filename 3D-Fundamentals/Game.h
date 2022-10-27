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
					gController = SDL_GameControllerOpen(0);
					for (size_t i = 0; i < SDL_NumJoysticks(); i++)
					{
						if (SDL_IsGameController(i)) {
							gController = SDL_GameControllerOpen(i);
						}
					}
				}
			}
		}
		else {
			isRunning = false;
		}
	}
	void Clean() {
		if (gController != NULL) {
			SDL_GameControllerClose(gController);
		}
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
		/*case SDL_CONTROLLERAXISMOTION:
			if (event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
				rotateLeft = false;
				rotateRight = true;
			}
			else if (event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
				rotateLeft = true;
				rotateRight = false;
			}
			break;*/
		case SDL_CONTROLLERBUTTONDOWN:
			if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
				rotateLeft = true;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
				rotateRight = true;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
				forward = true;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
				backward = true;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
				angle = 0;
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
				rotateLeft = false;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
				rotateRight = false;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
				forward = false;
			}
			else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
				backward = false;
			}
			break;
		default:
			break;
		}
	}
	void Update() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		Cube c(3.0f);
		auto ticks = SDL_GetTicks();
		auto lines = c.GetLines();
		for (Vec3<float>& v : lines.vertices) {
			angle += rotateLeft ? (M_PI / 170) : rotateRight ? -(M_PI / 170) : 0;
			Mat2<float>::RotateZ(v, WrapAngle((angle))); // roll first
			//Mat2<float>::RotateX(v, WrapAngle(((M_PI / 6)))); // then pitch
			//Mat2<float>::RotateY(v, WrapAngle(((M_PI / 6)))); // then yaw
			depth += forward ? 0.05f : backward ? -.05f : 0.0f;
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
	bool isRunning = false;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	NDCTransformer<float> transformer;
	SDL_GameController* gController = nullptr;
	double angle = 0;
	float depth = 6.0f;
	bool rotateLeft = false;
	bool rotateRight = false;
	bool forward = false;
	bool backward = false;
};

