#ifndef WORLD_H
#define WORLD_H
#include <entt/entt.hpp>
#include "gfx/masterrenderer.h"
#include "ecs/system/cameraSystem.h"

class World {
public:
	World(int x, int y, int width, int height);
	void update(float deltaTime);
	void render(float deltaTime);
	void setupScene();
	void setViewport(int x, int y, int width, int height);
	entt::registry &getECS();
	std::vector<std::unique_ptr<ComponentSystem>>& getSystems();
	entt::entity createEntity();
private:
	entt::registry m_Registry;
	entt::entity sponza, tank, crate, dirLight, pointLight, player;
	std::unique_ptr<MasterRenderer> masterRenderer;
	std::vector<std::unique_ptr<ComponentSystem>> systems;
	int m_x, m_y, m_width, m_height;

};

#endif