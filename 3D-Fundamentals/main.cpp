#include "Game.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
int main(int argc, char* argv[]) {
	Game game((const char*)"Game", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	SDL_DisplayMode current;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {

		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

		if (should_be_zero != 0)
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

		else
			// On success, print the current display mode.
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);

	}
	const auto tickTime = current.refresh_rate == 0 ? 1000 / 60 : 1000 / current.refresh_rate;
	game.Setup();
	while (game.IsRunning()) {
		const auto begin = SDL_GetTicks();
		game.Update();
		game.Render();
		const auto end = SDL_GetTicks();
		const auto diff = end - begin;
		if (diff < tickTime) {
			SDL_Delay(tickTime - diff);
		}
	}	
	return 0;
}