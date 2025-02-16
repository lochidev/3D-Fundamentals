#pragma once
#include "Scene.h"

class MainScene : public Scene<float>
{
public:
	MainScene(SDL_Renderer* r,
		const NDCTransformer<float>& t, const InputManager& ip, const int x,
		const int y) : Scene<float>(r, t, ip, x, y)
	{}
	void Update() override {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		Cube c(3.0f);
		auto ticks = SDL_GetTicks();
		auto lines = c.GetTriangles();
		if (inputManager.IsPressed(G_B_PRESSED)) {
			angle = 0;
		}
		for (Vec3& v : lines.vertices) {
			angle += inputManager.IsPressed(G_LEFT_PRESSED) ? (M_PI / 170)
				: inputManager.IsPressed(G_RIGHT_PRESSED) ? -(M_PI / 170) : 0;
			if (inputManager.IsPressed(G_A_PRESSED))
				Mat2<float>::RotateZ(v, Graphics::WrapAngle((angle))); // roll first
			if (inputManager.IsPressed(G_X_PRESSED))
				Mat2<float>::RotateX(v, Graphics::WrapAngle((angle))); // then pitch
			if (inputManager.IsPressed(G_Y_PRESSED))
				Mat2<float>::RotateY(v, Graphics::WrapAngle((angle))); // then yaw
			depth += inputManager.IsPressed(G_UP_PRESSED) ? 0.05f
				: inputManager.IsPressed(G_DOWN_PRESSED) ? -.05f : 0.0f;
			v += {0.0f, 0.0f, depth};
			transformer.TransformNDC(v);
		}
		for (auto i = lines.indexes.cbegin(),
			end = lines.indexes.cend();
			i != end; std::advance(i, 3)) {
			Vec3& v1 = lines.vertices[*i];
			Vec3& v2 = lines.vertices[*(std::next(i))];
			Vec3& v3 = lines.vertices[*(std::next(i, 2))];
			graphics->DrawTriangle(v1, v2, v3);
			//SDL_RenderPresent(renderer);
		}
		//DrawTriangle({ 0, 500 }, { 300, 300 }, { 1280, 400 });

	}
	void Render() override {
		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
private:
	/*SDL_Renderer* renderer;
	const NDCTransformer<float>& transformer;
	const InputManager& inputManager;*/
	double angle = 0;
	float depth = 6.0f;
};