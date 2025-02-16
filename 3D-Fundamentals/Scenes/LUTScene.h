#pragma once
#include "Scene.h"

class LUTScene : public Scene<float>
{
public:
	LUTScene(SDL_Renderer* r,
        const NDCTransformer<float>& t, const InputManager& ip, const int x,
        const int y) : Scene<float>(r, t, ip, x, y)
	{
	}
    void Update() override {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Assuming you have a TTF_Font* font initialized elsewhere
        TTF_Font* font = TTF_OpenFont("Assets/short-baby.ttf", 24);
        if (!font) {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            return;
        }

        // Create a Button with text and font
        SDL_Color buttonColor = { 0, 0, 255, 255 };  // Blue color
        SDL_Color textColor = { 255, 255, 0, 255 }; // Yellow text
        const int buttonY = (windowY / 12) * 11;
        Button b(150, buttonY, 200, 50, buttonColor, "Execute!", textColor, font);

        // Draw the button
        b.Draw(renderer);
        //draw horizontal border
        SDL_RenderDrawLine(renderer, (windowX / 10) * 6, 0, (windowX / 10) * 6, buttonY - 75);
        //draw horizontal line
        SDL_RenderDrawLine(renderer, 0, buttonY - 75, (windowX), buttonY - 75);

        // Get mouse state
        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        // Check if the button is clicked
        if (b.IsClicked(mouseX, mouseY, mouseState)) {
            SDL_Log("Button Clicked!");
        }

        // Clean up the font after using it
        TTF_CloseFont(font);
    }
	void Render() override {
		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
private:
	Uint32 xC = 0, yC = 0;
};