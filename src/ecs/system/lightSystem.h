#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H
#include "componentSystem.h"
#include "ecs/comp/directionallightComp.h"
#include <ecs/comp/pointlightComp.h>


class LightSystem : public ComponentSystem {
public:
	LightSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
};
#endif