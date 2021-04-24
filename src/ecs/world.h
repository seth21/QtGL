#ifndef WORLD_H
#define WORLD_H
#include <entt/entt.hpp>
class World {
public:
	World();
	void update(float deltaTime);

private:
	entt::registry m_Registry;
	entt::entity e;
};

#endif