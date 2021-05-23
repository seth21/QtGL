#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "componentSystem.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glm/mat4x4.hpp>

class RenderSystem : public ComponentSystem {
public:
	RenderSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
	
};

#endif