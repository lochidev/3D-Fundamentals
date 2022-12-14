#pragma once
#include "SDL.h"
#include "SDL_gamecontroller.h"
#include "LoggingTimer.h"

#define G_LEFT_PRESSED 0
#define G_RIGHT_PRESSED 1
#define G_UP_PRESSED 2
#define G_DOWN_PRESSED 3
#define G_X_PRESSED 4
#define G_Y_PRESSED 5
#define G_A_PRESSED 6
#define G_B_PRESSED 7

class InputManager {
public:
	InputManager() = default;
	void HandleEvents() {
		//LoggingTimer timer;
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			/*case SDL_CONTROLLERAXISMOTION:
				if (event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
					rotateLeft = false;
					rotateRight = true;
				}
				else if (event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
					rotateLeft = true;
					rotateRight = false;
				}
				break;*/
			break;
			/*case SDL_KEYDOWN:
				SDL_GameControllerGetButton(gController, [event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				SDL_GameControllerGetButton(gController, [event.key.keysym.sym] = false;
				break;*/
				/*case SDL_CONTROLLERBUTTONDOWN:
					SDL_GameControllerGetButton(gController, [event.cbutton.button] = 1;
					break;
				case SDL_CONTROLLERBUTTONUP:
					SDL_GameControllerGetButton(gController, [event.cbutton.button] = 0;
					break;*/
		default:
			break;
		}
		SDL_FlushEvents(SDL_APP_TERMINATING, SDL_LASTEVENT);
		/*auto i = SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY);
		SDL_Log("Value: #%d", i);*/

	}
	bool IsPressed(unsigned int t) const {
		const Uint8* keystates = SDL_GetKeyboardState(NULL);
		if (gController == NULL) {
			switch (t) {
			case G_LEFT_PRESSED:
				return keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT];

			case G_RIGHT_PRESSED:
				return keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT];

			case G_UP_PRESSED:
				return keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP];

			case G_DOWN_PRESSED:
				return keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN];

			case G_X_PRESSED:
				return keystates[SDL_SCANCODE_X];

			case G_Y_PRESSED:
				return keystates[SDL_SCANCODE_Z];

			case G_A_PRESSED:
				return keystates[SDL_SCANCODE_C];

			case G_B_PRESSED:
				return keystates[SDL_SCANCODE_B];

			default:
				break;
			}
		}
		else {
			switch (t) {
			case G_LEFT_PRESSED:
				return SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) 
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) 
					|| keystates[SDL_SCANCODE_A]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) || keystates[SDL_SCANCODE_LEFT]
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTX) < -1000
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTX) < -1000;
				  
			case G_RIGHT_PRESSED:
				return SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) 
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) 
					|| keystates[SDL_SCANCODE_D]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || keystates[SDL_SCANCODE_RIGHT] 
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTX) > 1000
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTX) > 1000;

			case G_UP_PRESSED:
				return keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_DPAD_UP)
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) < -1000
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTY) < -1000;

			case G_DOWN_PRESSED:
				return keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) > 1000
					|| SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_RIGHTY) > 1000;

			case G_X_PRESSED:
				return keystates[SDL_SCANCODE_X]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_X);

			case G_Y_PRESSED:
				return keystates[SDL_SCANCODE_Z]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_Y);

			case G_A_PRESSED:
				return keystates[SDL_SCANCODE_C]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_A);

			case G_B_PRESSED:
				return keystates[SDL_SCANCODE_B]
					|| SDL_GameControllerGetButton(gController, SDL_CONTROLLER_BUTTON_B);

			default:
				break;
			}
		}

	}
public:
	bool isRunning = false;
	SDL_GameController* gController = nullptr;
};