#include "movementSystem.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/moveComp.h"
#include "ecs/comp/hierarchyComp.h"
#include <QDebug>

MovementSystem::MovementSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void MovementSystem::update(float deltaTime)
{
	//Sort the movement comps based on hierarchy depth
	m_ecs->sort<MoveComp>([this](const entt::entity lhs, const entt::entity rhs) {
		HierarchyComp* lh = m_ecs->try_get<HierarchyComp>(lhs);
		HierarchyComp* rh = m_ecs->try_get<HierarchyComp>(rhs);
		if (lh && rh) return lh->hierarchyDepth < rh->hierarchyDepth;
		else return false;
		});
	//Apply the movement to the transforms
	auto view = m_ecs->view<MoveComp, TransformComp>();
	for (auto entity : view) {
		auto [move, transform] = view.get<MoveComp, TransformComp>(entity);
		bool changed = false;
		if (!isVec3Zero(move.scale)) {
			transform.localScale = move.scale;
			changed = true;
		}
		if (!isVec3Zero(move.angularVelocity)) {
			TransformSystem::rotate(*m_ecs, &transform, move.angularVelocity, move.angularVelRelativeToSelf);
			changed = true;
		}
		if (!isVec3Zero(move.velocity)) {
			TransformSystem::translate(*m_ecs, &transform, move.velocity, move.velRelativeToSelf);
			changed = true;
		}
		
		if (changed) {
			m_ecs->patch<TransformComp>(entity);
			if (move.clearAfterMove) {
				move.velocity = glm::vec3(0);
				move.angularVelocity = glm::vec3(0);
				move.scale = glm::vec3(0);
			}
		}
		
	}

}

bool MovementSystem::isVec3Zero(glm::vec3& vec3)
{
	return vec3.x == vec3.y == vec3.z == 0;
}
