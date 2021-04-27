#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H
#include <entt/entt.hpp>
#include "gfx/masterrenderer.h"

class ComponentSystem {
public:
	ComponentSystem(entt::registry* registry, MasterRenderer* masterRenderer) {
		m_ecs = registry;
		m_renderer = masterRenderer;
	};
	virtual void update(float deltaTime) {};
protected:
	entt::registry* m_ecs;
	MasterRenderer* m_renderer;
};

#endif