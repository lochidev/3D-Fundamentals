#include "Game.h"
int main(int argc, char* argv[]) {
	Game game((const char*)"MONEY MONEY BICTHES", 1280, 720, false);
	while (game.isRunning) {
		game.HandleEvents();
		game.Update();
		game.Render();
	}	
	game.Clean();
	return 0;
}