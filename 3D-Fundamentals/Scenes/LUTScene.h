#pragma once
#include "Scene.h"

class LUTScene : public Scene<float>
{
public:
	LUTScene(SDL_Renderer* r,
		const NDCTransformer<float>& t, const InputManager& ip) : Scene<float>(r, t, ip)
	{
	}
	void Update() override {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		Button b(100, 100, 100, 100, { 0, 0, 255 });
		b.Draw(renderer);
		if (b.IsClicked(100, 100, SDL_GetMouseState(NULL, NULL)))
		{
			SDL_Log("Button Clicked!");
		}
		//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // set to blue
		////draw a button with SDL and detect when clicked. 
		//int x, y;
		//Uint32 buttons = SDL_GetMouseState(&x, &y);

		//if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		//	if (xC != 0 && yC != 0)
		//		SDL_RenderDrawLine(renderer, xC, yC, x, y);
		//	else
		//	{
		//		SDL_RenderDrawPoint(renderer, x, y);
		//	}
		//	xC = x;
		//	yC = y;
		//}

		//if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		//	if (xC != 0 && yC != 0)
		//		SDL_RenderDrawLine(renderer, xC, yC, x, y);
		//	else
		//	{
		//		SDL_RenderDrawPoint(renderer, x, y);
		//		
		//	}
		//	xC = x;
		//	yC = y;
		//}
	}
	void Render() override {
		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
private:
	Uint32 xC = 0, yC = 0;
};