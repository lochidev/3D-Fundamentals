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

public:
    Logic(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int width, int height, SDL_Color color = { 255, 255, 255, 255 })
        : renderer(renderer), texture(nullptr), width(width), height(height), color(color) {
        loadTexture(imagePath);
        // Adjust position to use the center of the image as origin
        this->x = x - width / 2;
        this->y = y - height / 2;
    }

    virtual ~Logic() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    void loadTexture(const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << path << " SDL_Error: " << IMG_GetError() << std::endl;
            return;
        }

        // Convert all non-transparent pixels to the specified color
        Uint32* pixels = (Uint32*)surface->pixels;
        SDL_PixelFormat* format = surface->format;
        Uint32 transparent = SDL_MapRGBA(format, 0, 0, 0, 0);

        for (int i = 0; i < surface->w * surface->h; ++i) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixels[i], format, &r, &g, &b, &a);
            if (a > 0) { // Only change non-transparent pixels
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
    }
};

class AND : public Logic {
public:
    AND(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/AND.png", x, y, width, height, color) {}
};

class OR : public Logic {
public:
    OR(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/OR.png", x, y, width, height, color) {}
};

class NOT : public Logic {
public:
    NOT(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/NOT.png", x, y, width, height, color) {}
};

class XOR : public Logic {
public:
    XOR(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color = { 255, 255, 255, 255 })
        : Logic(renderer, "Assets/logic-sprites/XOR.png", x, y, width, height, color) {}
};