#pragma once
#include "action.h"

class InputHandler {
public:
	virtual void handleAction(ACTION action, bool pressed) = 0;
	virtual void handleMouseDelta(int dX, int dY) = 0;
};