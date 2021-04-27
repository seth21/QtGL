#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "ecs/comp/cameraComp.h"
#include "componentSystem.h"
#include "inputhandler.h"

class CameraSystem : public ComponentSystem, InputHandler{
public:
	CameraSystem(entt::registry *registry, MasterRenderer *masterRenderer);
	void update(float deltaTime);
	virtual void handleAction(ACTION action, bool pressed) override;
	virtual void handleMouseDelta(int dx, int dy) override;
private:
	int m_dx = 0, m_dy = 0;

};
#endif