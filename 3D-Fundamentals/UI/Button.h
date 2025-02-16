#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>

// Helper function to draw a filled circle
void DrawFilledCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

class Button {
private:
    SDL_Rect rect;
    SDL_Color color;
    bool isPressed;
    std::string text;
    SDL_Color textColor;
    TTF_Font* font;
    int cornerRadius; // Radius for rounded corners

public:
    // Constructor
    Button(int x, int y, int w, int h, SDL_Color c, const std::string& t, SDL_Color tc, TTF_Font* f, int radius = 10)
        : rect{ x, y, w, h }, color{ c }, isPressed{ false }, text{ t }, textColor{ tc }, font{ f }, cornerRadius{ radius } {
    }

    // Draw button with rounded corners
    void Draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        // Draw corner circles
        DrawFilledCircle(renderer, rect.x + cornerRadius, rect.y + cornerRadius, cornerRadius, color); // Top-left
        DrawFilledCircle(renderer, rect.x + rect.w - cornerRadius, rect.y + cornerRadius, cornerRadius, color); // Top-right
        DrawFilledCircle(renderer, rect.x + cornerRadius, rect.y + rect.h - cornerRadius, cornerRadius, color); // Bottom-left
        DrawFilledCircle(renderer, rect.x + rect.w - cornerRadius, rect.y + rect.h - cornerRadius, cornerRadius, color); // Bottom-right

        // Draw main body (rectangles to fill gaps)
        SDL_Rect topRect = { rect.x + cornerRadius, rect.y, rect.w - 2 * cornerRadius, cornerRadius };
        SDL_Rect bottomRect = { rect.x + cornerRadius, rect.y + rect.h - cornerRadius, rect.w - 2 * cornerRadius, cornerRadius };
        SDL_Rect leftRect = { rect.x, rect.y + cornerRadius, cornerRadius, rect.h - 2 * cornerRadius };
        SDL_Rect rightRect = { rect.x + rect.w - cornerRadius, rect.y + cornerRadius, cornerRadius, rect.h - 2 * cornerRadius };
        SDL_Rect centerRect = { rect.x + cornerRadius, rect.y + cornerRadius, rect.w - 2 * cornerRadius, rect.h - 2 * cornerRadius };

        SDL_RenderFillRect(renderer, &topRect);
        SDL_RenderFillRect(renderer, &bottomRect);
        SDL_RenderFillRect(renderer, &leftRect);
        SDL_RenderFillRect(renderer, &rightRect);
        SDL_RenderFillRect(renderer, &centerRect);

        // Render text
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
