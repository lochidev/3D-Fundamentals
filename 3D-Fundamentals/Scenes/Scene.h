#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include "../InputManager.h"
#include "../NDCTransformer.h"
#include "../UI/Button.h"
#include "Graphics.h"
template <typename T>
class Scene {
public:
	Scene(SDL_Renderer* r,
		const NDCTransformer<T>& t,
		const InputManager& ip,
		const int x,
		const int y) : renderer(r), transformer(t), inputManager(ip), windowX(x), windowY(y)
	{}
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleEvents() = 0;
protected:
	SDL_Renderer* renderer;
	const NDCTransformer<T>& transformer;
	const InputManager& inputManager;
	const int windowX;
	const int windowY;
};