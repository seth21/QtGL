#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H
#include "componentSystem.h"



class LightSystem : public ComponentSystem {
public:
	LightSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
};
#endif