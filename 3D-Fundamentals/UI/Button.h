#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Button {
private:
    SDL_Rect rect;
    SDL_Color color;
    bool isPressed;
    std::string text;
    SDL_Color textColor;
    TTF_Font* font;

public:
    // Constructor
    Button(int x, int y, int w, int h, SDL_Color c, const std::string& t, SDL_Color tc, TTF_Font* f)
        : rect{ x, y, w, h }, color{ c }, isPressed{ false }, text{ t }, textColor{ tc }, font{ f } {
    }

    // Draw button
    void Draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);

        if (font) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
            if (textSurface) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect textRect = { rect.x + (rect.w - textSurface->w) / 2, rect.y + (rect.h - textSurface->h) / 2, textSurface->w, textSurface->h };
                    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }
        }
    }

    // Check if button is clicked
    bool IsClicked(int mouseX, int mouseY, Uint32 mouseState) {
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h);
        }
        return false;
    }
};
