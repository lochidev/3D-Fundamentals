#pragma once
#include "Scene.h"
#include "../Logic.h"
#include <SDL.h>
#include <vector>

class LUTScene : public Scene<float> {
public:
	LUTScene(SDL_Renderer* r, const NDCTransformer<float>& t, const InputManager& ip, const int x, const int y)
		: Scene<float>(r, t, ip, x, y), selectedGate(nullptr), button(nullptr) {

		// Load font once
		font = TTF_OpenFont("Assets/fonts/short-baby.ttf", 24);
		if (!font) {
			SDL_Log("Failed to load font: %s", TTF_GetError());
		}

		// Initialize button
		SDL_Color blue = { 0, 0, 255, 255 };
		SDL_Color yellow = { 255, 255, 0, 255 };
		button = new Button(150, buttonY, 200, 50, blue, "Execute!", yellow, font);

		// Initialize logic gates
		_uiLogicGates.emplace_back(new AND(renderer, 400, buttonY, 200, 50, true, yellow));
		_uiLogicGates.emplace_back(new OR(renderer, 650, buttonY, 200, 50, true, yellow));
		_uiLogicGates.emplace_back(new XOR(renderer, 900, buttonY, 200, 50, true, yellow));
		_uiLogicGates.emplace_back(new NOT(renderer, 1150, buttonY, 200, 50, true, yellow));
	}

	~LUTScene() {
		if (font) {
			TTF_CloseFont(font);
		}
		for (auto* gate : _uiLogicGates) {
			delete gate;
		}
		for (auto* gate : placedGates) {
			delete gate;
		}
		if (button) {
			delete button;
		}
	}

	void Update() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		button->Draw(renderer);  // Access button via pointer
		for (auto* gate : _uiLogicGates) {
			gate->Draw();
		}
		for (auto* gate : placedGates) {
			gate->Draw();  // Draw placed gates as well
		}

		SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);

		// Draw horizontal borders
		SDL_RenderDrawLine(renderer, (windowX / 10) * 6, 0, (windowX / 10) * 6, buttonY - 75);
		SDL_RenderDrawLine(renderer, 0, buttonY - 75, windowX, buttonY - 75);

	}

	void HandleEvents() override {
		// Get mouse state
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_ESCAPE]) {
			selectedGate->SetSelected(false);
			//selectedGate = nullptr;
		}
		int mouseX, mouseY;
		Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		// Check if a logic gate from the initial set is clicked to select
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {

			for (auto* gate : _uiLogicGates) {
				if (gate->IsClicked(mouseX, mouseY, mouseState)) {
					if (selectedGate) {
						selectedGate->SetSelected(false);
					}
					selectedGate = gate;
					selectedGate->SetSelected(true);
					break;
				}
			}
		}

		if (!selectedGate) {
			for (auto* gate : placedGates) {
				if (gate->IsClicked(mouseX, mouseY, mouseState)) {
					if (selectedGate) {
						selectedGate->SetSelected(false);
					}
					selectedGate = gate;
					selectedGate->SetSelected(true);
					break;
				}
			}
		}
		// Handle deletion of placed logic gates with the delete key
		if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DELETE]) {
			if (selectedGate && !selectedGate->CanPlace()) {
				// Only delete if the selected gate is a placed gate
				auto it = std::find(placedGates.begin(), placedGates.end(), selectedGate);
				if (it != placedGates.end()) {
					placedGates.erase(it);  // Remove from placedGates vector
					selectedGate->Hide();
					delete selectedGate;    // Delete the gate object
					selectedGate = nullptr; // Reset the selected gate
				}
			}
		}

		// Check if the user clicks to place the selected gate
		if (selectedGate && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			// Restrict placement to the top-left area and ensure the gate is within the screen bounds
			if (mouseX <= (windowX / 10) * 6 && mouseY <= (windowY / 12) * 11) {
				int placedX = mouseX - selectedGate->GetWidth() / 2;
				int placedY = mouseY - selectedGate->GetHeight() / 2;

				// Make sure placedY doesn't go below the bottom border
				if (placedY + selectedGate->GetHeight() > buttonY - 75) {
					placedY = buttonY - 75 - selectedGate->GetHeight();  // Place at the border
				}
				if (selectedGate->CanPlace()) {
					// Create a new gate of the selected type and place it in the left space
					Logic* newGate = nullptr;
					if (dynamic_cast<AND*>(selectedGate)) {
						newGate = new AND(renderer, placedX, placedY, selectedGate->GetWidth(), selectedGate->GetHeight(), false, blue);
					}
					else if (dynamic_cast<OR*>(selectedGate)) {
						newGate = new OR(renderer, placedX, placedY, selectedGate->GetWidth(), selectedGate->GetHeight(), false, blue);
					}
					else if (dynamic_cast<XOR*>(selectedGate)) {
						newGate = new XOR(renderer, placedX, placedY, selectedGate->GetWidth(), selectedGate->GetHeight(), false, blue);
					}
					else if (dynamic_cast<NOT*>(selectedGate)) {
						newGate = new NOT(renderer, placedX, placedY, selectedGate->GetWidth(), selectedGate->GetHeight(), false, blue);
					}
					if (newGate) {
						placedGates.push_back(newGate);
					}
					selectedGate->SetSelected(false);  // Deselect after placing
					selectedGate = nullptr;  // Reset selected gate
				}
				else {
					selectedGate->Hide();
					selectedGate->SetPosition(placedX, placedY);
					selectedGate->Show();
				}
			}
		}
		return;
	}

	void Render() override {
		SDL_RenderPresent(renderer);
	}

private:
	static constexpr SDL_Color blue = { 0, 0, 255, 255 };
	static constexpr SDL_Color yellow = { 255, 255, 0, 255 };
	TTF_Font* font = nullptr;
	Button* button;
	Logic* selectedGate = nullptr;
	std::vector<Logic*> _uiLogicGates;
	std::vector<Logic*> placedGates;  // New vector to track placed logic gates
	const int buttonY = (windowY / 12) * 11;
};
