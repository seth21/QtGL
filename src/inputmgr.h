#pragma once
#include <unordered_map>
#include <algorithm>
#include <QKeyEvent>
#include "inputhandler.h"
#include "action.h"
#include <glm/vec2.hpp>

class InputManager {
public:
	static InputManager& getInstance()
	{
		static InputManager instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;

	void registerPress(int key, bool press);
	void registerMouse(int x, int y);
	bool isPressed(ACTION action);
	const std::unordered_map<int, ACTION>& getBindings();
	void setBinding(ACTION action, int key);
	void registerHandler(InputHandler *handler);
	void removeHandler(InputHandler* handler);
	glm::vec2 getMousePos();
	std::vector<InputHandler*> handlers;
	glm::vec2 lastMouse = glm::vec2(-999,-999);
private:
	InputManager() {
		keyBindings[Qt::Key_W] = FORWARD;
		keyBindings[Qt::Key_A] = LEFT;
		keyBindings[Qt::Key_D] = RIGHT;
		keyBindings[Qt::Key_S] = BACK;
		keyBindings[Qt::Key_Q] = UP;
		keyBindings[Qt::Key_E] = DOWN;
		keyBindings[Qt::Key_Z] = TREEUP;
		keyBindings[Qt::Key_X] = TREEDOWN;
	}

	std::unordered_map<int, ACTION> keyBindings;
	bool pressed[MAX_VALUE];

};
