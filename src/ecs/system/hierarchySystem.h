#ifndef HIERARCHYSYSTEM_H
#define HIERARCHYSYSTEM_H
#include "componentSystem.h"
#include "ecs/comp/dirtyTransformComp.h"
#include "ecs/comp/hierarchyComp.h"

class HierarchySystem : public ComponentSystem {
public:
	HierarchySystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
	
	unsigned char calculateHierarchyDepth(HierarchyComp *hier);
	void updateFamilyDepths(entt::registry& registry, entt::entity e);
	void hierarchyChanged(entt::registry& registry, entt::entity e);
	void hierarchyAdded(entt::registry& registry, entt::entity e);
	void hierarchyRemoved(entt::registry& registry, entt::entity e);
private:
	bool removeChildFromParent(entt::registry& registry, entt::entity parent, entt::entity child);
	void addChildToParent(entt::registry& registry, entt::entity parent, entt::entity child);
};
#endif