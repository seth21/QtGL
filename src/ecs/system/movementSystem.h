#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "componentSystem.h"
#include "ecs/system/transformSystem.h"


class MovementSystem : public ComponentSystem {
public:
	MovementSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
private:
	bool isVec3Zero(glm::vec3& vec3);
};
#endif