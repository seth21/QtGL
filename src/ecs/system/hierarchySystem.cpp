#include "hierarchySystem.h"
#include "inputmgr.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ecs/system/transformSystem.h>
#include "ecs/comp/moveComp.h"
HierarchySystem::HierarchySystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
	registry->on_construct<HierarchyComp>().connect<&HierarchySystem::hierarchyAdded>(this);
	registry->on_update<HierarchyComp>().connect<&HierarchySystem::hierarchyChanged>(this);
	registry->on_destroy<HierarchyComp>().connect<&HierarchySystem::hierarchyRemoved>(this);
	
}

void HierarchySystem::update(float deltaTime)
{
	
	
}



unsigned char HierarchySystem::calculateHierarchyDepth(HierarchyComp* hier)
{
	if (!hier) return 0;
	int depth = 0;
	entt::entity parent = hier->getParent();
	while (parent != entt::null) {
		depth++;
		auto parentComp = m_ecs->try_get<HierarchyComp>(parent);
		if (parentComp) parent = parentComp->getParent();
		else break;
	}
	hier->hierarchyDepth = depth;
}

void HierarchySystem::updateFamilyDepths(entt::registry& registry, entt::entity e)
{
	auto hier = registry.try_get<HierarchyComp>(e);
	if (hier) {
		calculateHierarchyDepth(hier);
		for (auto child : hier->getChildren()) {
			updateFamilyDepths(registry, child);
			
		}
	}
	
}

void HierarchySystem::hierarchyChanged(entt::registry& registry, entt::entity e)
{
	//Did parent change?
	auto hier = registry.try_get<HierarchyComp>(e);
	if (hier && hier->parentChanged()) {
		//Remove from old parent
		removeChildFromParent(registry, hier->getPreviousParent(), e);
		hier->invalidateParentChange();
		//Add to new parent
		addChildToParent(registry, hier->getParent(), e);
	}
	//Recalculate depth of this and children
	updateFamilyDepths(registry, e);
	//Add a dirty transform comp to this(which will also be added to children) to recalculate matrices
	if (!registry.any_of<DirtyTransformComp>(e)) {
		registry.emplace<DirtyTransformComp>(e);
	}

}

void HierarchySystem::hierarchyAdded(entt::registry& registry, entt::entity e)
{
	//Add the entity to the parent as well
	auto hier = registry.try_get<HierarchyComp>(e);
	if (hier) addChildToParent(registry, hier->getParent(), e);
	
	//Recalculate depth of this and children
	updateFamilyDepths(registry, e);
	//Add a dirty transform comp to this(which will also be added to children) to recalculate matrices
	if (!registry.any_of<DirtyTransformComp>(e)) {
		registry.emplace<DirtyTransformComp>(e);
	}
	
}

void HierarchySystem::hierarchyRemoved(entt::registry& registry, entt::entity e)
{
	//Remove children as well (this will be called recursively)
	auto hier = registry.try_get<HierarchyComp>(e);
	if (hier) {
		for (auto childE : hier->getChildren()) {
			registry.destroy(childE);
		}
		//Also remove this from the parent's children
		removeChildFromParent(registry, hier->getParent(), e);	
	}	
}

bool HierarchySystem::removeChildFromParent(entt::registry& registry, entt::entity parent, entt::entity child)
{
	if (parent == entt::null || child == entt::null) return false;
	auto hierParent = registry.try_get<HierarchyComp>(parent);
	if (hierParent) {
		auto& parentsChildren = hierParent->getChildren();
		auto position = std::find(parentsChildren.begin(), parentsChildren.end(), child);
		if (position != parentsChildren.end()) // == myVector.end() means the element was not found
		{
			parentsChildren.erase(position);
			return true;
		}
	}
	return false;
}

void HierarchySystem::addChildToParent(entt::registry& registry, entt::entity parent, entt::entity child)
{
	if (parent == entt::null || child == entt::null) return;
	auto hierParent = registry.try_get<HierarchyComp>(parent);
	if (hierParent) {
		auto& parentsChildren = hierParent->getChildren();
		if (std::find(parentsChildren.begin(), parentsChildren.end(), child) == parentsChildren.end()) {
			parentsChildren.push_back(child);
		}
	}
	
}
