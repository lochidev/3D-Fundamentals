#pragma once
#include "SDL.h"
#include "../InputManager.h"
#include "../NDCTransformer.h"
#include "../UI/Button.h"
#include "Graphics.h"
template <typename T>
class Scene {
public:
	Scene(SDL_Renderer* r,
		const NDCTransformer<T>& t,
		const InputManager& ip) : renderer(r), transformer(t), inputManager(ip)
	{}
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	SDL_Renderer* renderer;
	const NDCTransformer<T>& transformer;
	const InputManager& inputManager;

};