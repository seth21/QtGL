#include "inputmgr.h"

#include <QDebug>


void InputManager::registerPress(int key, bool press)
{
    auto it = keyBindings.find(key);
    if (it != keyBindings.end()) {
        pressed[it->second] = press;
        
        for (int i = 0; i < handlers.size(); i++) {
            handlers[i]->handleAction(it->second, press);
        }
    }

    
}

void InputManager::registerMouse(int x, int y)
{
    if ((lastMouse.x == -999 && lastMouse.y == -999)) {
        lastMouse.x = x;
        lastMouse.y = y;
    }
    else {
        //notify handlers
        int dx = x - lastMouse.x;
        int dy = y - lastMouse.y;
        //update pos
        lastMouse.x = x;
        lastMouse.y = y;

        for (int i = 0; i < handlers.size(); i++) {
            handlers[i]->handleMouseDelta(dx, dy);
        }
    }
    
}

bool InputManager::isPressed(ACTION action)
{
    if (action == MAX_VALUE) return false;
    return pressed[action];
}

const std::unordered_map<int, ACTION>& InputManager::getBindings()
{
    return keyBindings;
}

void InputManager::setBinding(ACTION action, int key)
{
    if (action == MAX_VALUE) return;
    keyBindings[key] = action;
}

void InputManager::registerHandler(InputHandler *handler)
{
    handlers.push_back(handler);
}

void InputManager::removeHandler(InputHandler* handler)
{
    handlers.erase(std::remove(handlers.begin(), handlers.end(), handler), handlers.end());
}

glm::vec2 InputManager::getMousePos()
{
    return lastMouse;
}
