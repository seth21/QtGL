#ifndef TRANSFORMUNDIRTYSYSTEM_H
#define TRANSFORMUNDIRTYSYSTEM_H
#include "componentSystem.h"
#include "ecs/comp/transformComp.h"
#include <glm/gtc/matrix_transform.hpp>


class TransformUndirtySystem : public ComponentSystem {
public:
	TransformUndirtySystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
};
#endif