#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
#include "componentSystem.h"
#include "ecs/comp/transformComp.h"
#include <glm/gtc/matrix_transform.hpp>


class TransformSystem : public ComponentSystem {
public:
	TransformSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
	bool first = false;
};
#endif