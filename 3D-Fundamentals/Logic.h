#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

class Logic {
protected:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int x, y, width, height;
    SDL_Color color;
    bool selected;
    const int padding = 5; // Padding for selection border
    bool _canPlace = false;
public:
    Logic(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int width, int height, int canPlace, SDL_Color color = { 255, 255, 255, 255 })
        : renderer(renderer), texture(nullptr), width(width), height(height), color(color), selected(false), _canPlace(canPlace) {
        loadTexture(imagePath);
        this->x = x - width / 2;
        this->y = y - height / 2;
    }

    virtual ~Logic() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
    bool CanPlace() {
        return _canPlace;
    }
    void SetPosition(int xx, int yy) {
        x = xx;
        y = yy;
    }
    void loadTexture(const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << path << " SDL_Error: " << IMG_GetError() << std::endl;
            return;
        }

        Uint32* pixels = (Uint32*)surface->pixels;
        SDL_PixelFormat* format = surface->format;
        Uint32 transparent = SDL_MapRGBA(format, 0, 0, 0, 0);

        for (int i = 0; i < surface->w * surface->h; ++i) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixels[i], format, &r, &g, &b, &a);
            if (a > 0) {
                pixels[i] = SDL_MapRGBA(format, color.r, color.g, color.b, a);
            }
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    virtual void Draw() {
        if (texture) {
            SDL_Rect dstRect = { x, y, width, height };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        }

        if (selected) {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_Rect borderRect = { x - padding, y - padding, width + 2 * padding, height + 2 * padding };
            SDL_RenderDrawRect(renderer, &borderRect);
        }
    }

    bool IsClicked(int mouseX, int mouseY, Uint32 mouseState) {
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            return (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
        }
        return false;
    }

    void SetSelected(bool isSelected) {
        selected = isSelected;
    }

    // Getter functions for width and height
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    // Hide function to stop drawing
    void Hide() {
        // Optionally, we can set a flag here to stop rendering
        selected = false;  // This will deselect the gate
    }

    // Show function to make the gate visible again
    void Show() {
        selected = true;  // This will select the gate and make it visible
    }
};

class AND : public Logic {
public:
    AND(SDL_Renderer* renderer, int x, int y, int width, int height, bool canPlace, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/AND.png", x, y, width, height, canPlace, color) {}
};

class OR : public Logic {
public:
    OR(SDL_Renderer* renderer, int x, int y, int width, int height, bool canPlace, SDL_Color color = { 255, 255, 0, 255 })
        : Logic(renderer, "Assets/logic-sprites/OR.png", x, y, width, height, canPlace, color) {}
};

class NOT : public Logic {
public:
    NOT(SDL_Renderer* renderer, int x, int y, int width, int height, bool canPlace, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/NOT.png", x, y, width, height, canPlace, color) {}
};

class XOR : public Logic {
public:
    XOR(SDL_Renderer* renderer, int x, int y, int width, int height, bool canPlace, SDL_Color color = { 255, 255, 0, 255 })
        : Logic(renderer, "Assets/logic-sprites/XOR.png", x, y, width, height, canPlace, color) {}
};

