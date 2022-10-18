#include "Game.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
int main(int argc, char* argv[]) {
	Game game((const char*)"Game", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	while (game.isRunning) {
		game.HandleEvents();
		game.Update();
		game.Render();
	}	
	game.Clean();
	return 0;
}