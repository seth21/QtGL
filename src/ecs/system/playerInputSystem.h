#ifndef PLAYERINPUTSYSTEM_H
#define PLAYERINPUTSYSTEM_H
#include "componentSystem.h"
#include <glm/vec3.hpp>


class PlayerInputSystem : public ComponentSystem {
public:
	PlayerInputSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);

};

#endif