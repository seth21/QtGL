#include "transformSystem.h"

TransformSystem::TransformSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
	registry->on_construct<TransformComp>().connect<&TransformSystem::transformAdded>(this);
	registry->on_update<TransformComp>().connect<&TransformSystem::transformChanged>(this);
	registry->on_destroy<TransformComp>().connect<&TransformSystem::transformRemoved>(this);

	registry->on_construct<DirtyTransformComp>().connect<&TransformSystem::dirtyAdded>(this);
}

void TransformSystem::update(float deltaTime)
{
	//Sort dirty transforms based on their hierarchy depth
	m_ecs->sort<DirtyTransformComp>([this](const entt::entity lhs, const entt::entity rhs) {
		HierarchyComp *lh = m_ecs->try_get<HierarchyComp>(lhs);
		HierarchyComp* rh = m_ecs->try_get<HierarchyComp>(rhs);
		if (lh && rh) return lh->hierarchyDepth < rh->hierarchyDepth;
		else return false;
		});
	
	auto view = m_ecs->view<DirtyTransformComp>();
	for (auto entity : view) {
		TransformComp *transComp = m_ecs->try_get<TransformComp>(entity);
		if (transComp) {
			glm::mat4 scl = glm::scale(glm::mat4(1.0f), transComp->localScale);
			glm::mat4 rot = glm::mat4_cast(transComp->localOrientation);
			glm::mat4 trans = glm::translate(glm::mat4(1.0), transComp->localPosition);
			auto hierComp = m_ecs->try_get<HierarchyComp>(entity);
			if (hierComp && hierComp->getParent() != entt::null) {
				TransformComp* parentTrans = m_ecs->try_get<TransformComp>(hierComp->getParent());
				if (parentTrans) {
					transComp->transformMatrix = parentTrans->transformMatrix * trans * rot * scl;
					transComp->inverseTransformMatrix = glm::inverse(transComp->transformMatrix);
					transComp->worldOrientation = parentTrans->worldOrientation * transComp->localOrientation;
					transComp->worldOrientationInverse = glm::conjugate(transComp->worldOrientation);
					transComp->worldScale = parentTrans->worldScale * transComp->localScale;
				}
				else qDebug() << "ERROR - Hierarchy component pointing to non-existent parent Transform component!";

				
			}
			else {
				transComp->transformMatrix = trans * rot * scl;
				transComp->inverseTransformMatrix = glm::inverse(transComp->transformMatrix);
				//transComp->parentOrientationInverse = glm::quat(1.0, 0.0, 0.0, 0.0);
				transComp->worldOrientation = transComp->localOrientation;
				transComp->worldOrientationInverse = glm::conjugate(transComp->worldOrientation);
				transComp->worldScale = transComp->localScale;
				//qDebug() << "Updated Dirty Entity:" << std::to_string((int)entity).c_str();
			}
		}
		else qDebug() << "ERROR - Dirty Transform without Transform component!";
	}
	//first = true;

	m_ecs->clear<DirtyTransformComp>();
}

void TransformSystem::transformChanged(entt::registry& registry, entt::entity e)
{
	//qDebug() << "TRANSFORM CHANGED" << (int)e;
	if (!registry.any_of<DirtyTransformComp>(e)) {
		registry.emplace<DirtyTransformComp>(e);
	}
	
}

void TransformSystem::transformAdded(entt::registry& registry, entt::entity e)
{
	//qDebug() << "TRANSFORM ADDED" << (int)e;
	if (!registry.any_of<DirtyTransformComp>(e)) {
		registry.emplace<DirtyTransformComp>(e);
	}
}

void TransformSystem::transformRemoved(entt::registry& registry, entt::entity e)
{
	//qDebug() << "TRANSFORM REMOVED" << (int)e;
	//Remove children as well (this will be called recursively)
	auto hier = registry.try_get<HierarchyComp>(e);
	if (hier) {
		for (auto childE : hier->getChildren()) {
			registry.destroy(childE);
		}
	}
	
}

void TransformSystem::dirtyAdded(entt::registry& registry, entt::entity e)
{
	//When a dirty component is added to an entity, add it to the children as well recursively
	HierarchyComp* hier = registry.try_get<HierarchyComp>(e);
	if (hier) {
		for (auto child : hier->getChildren()) {
			if (!registry.any_of<DirtyTransformComp>(child)) {
				registry.emplace<DirtyTransformComp>(child);
			}
		}
	}
}

glm::vec3 TransformSystem::getWorldPosition(entt::registry& registry, TransformComp* trans) {
	return pointLocalToWorld(registry, trans, trans->localPosition);
}

void TransformSystem::setWorldPosition(entt::registry& registry, TransformComp* trans, glm::vec3 worldPos) {
	trans->localPosition = pointWorldToLocal(registry, trans, worldPos);
}

glm::quat TransformSystem::getWorldOrientation(entt::registry& registry, TransformComp* trans)
{
	return orientationLocalToWorld(registry, trans, trans->localOrientation);
}

void TransformSystem::setWorldOrientation(entt::registry& registry, TransformComp* trans, glm::quat worldOrientation)
{
	trans->localOrientation = orientationWorldToLocal(registry, trans, worldOrientation);
}

glm::vec3 TransformSystem::getWorldScale(TransformComp* trans)
{
	return trans->worldScale;
}

void TransformSystem::rotate(entt::registry& registry, TransformComp* trans, glm::vec3 eulerDegrees, bool relativeToSelf)
{
	if (relativeToSelf) {
		trans->localOrientation = glm::quat(eulerDegrees) * trans->localOrientation;
	}
	else {
		glm::quat worldRot = TransformSystem::getWorldOrientation(registry, trans);
		worldRot = glm::quat(eulerDegrees) * worldRot;
		TransformSystem::setWorldOrientation(registry, trans, worldRot);
	}
}

void TransformSystem::rotateAround(entt::registry& registry, TransformComp* trans, glm::vec3 point, glm::vec3 axis, float angleDegrees)
{
	//TODO
}

void TransformSystem::translate(entt::registry& registry, TransformComp* trans, glm::vec3 translation, bool relativeToSelf)
{
	if (relativeToSelf) {
		trans->localPosition = trans->localPosition + translation;
	}
	else {
		glm::vec3 newWorldPos = TransformSystem::getWorldPosition(registry, trans) + translation;
		TransformSystem::setWorldPosition(registry, trans, newWorldPos);
	}
}

void TransformSystem::lookAt(entt::registry& registry, TransformComp* trans, glm::vec3 normalizedDirection, glm::vec3 worldUp)
{
	setWorldOrientation(registry, trans, glm::quatLookAt(normalizedDirection, worldUp));
}

glm::vec3 TransformSystem::getForward(entt::registry& registry, TransformComp* trans)
{
	return trans->localOrientation * glm::vec3(0,0,-1);
}

glm::vec3 TransformSystem::pointLocalToWorld(entt::registry& registry, TransformComp* trans, glm::vec3 localPoint)
{
	auto entity = entt::to_entity(registry, *trans);
	auto hierComp = registry.try_get<HierarchyComp>(entity);
	if (hierComp && hierComp->getParent() != entt::null) {
		TransformComp* parentTrans = registry.try_get<TransformComp>(hierComp->getParent());
		if (parentTrans) {
			return parentTrans->transformMatrix * glm::vec4(localPoint, 1);
			//trans->worldOrientation = worldOrientation;
		}
	}
	return localPoint;
}

glm::vec3 TransformSystem::pointWorldToLocal(entt::registry& registry, TransformComp* trans, glm::vec3 worldPoint)
{
	auto entity = entt::to_entity(registry, *trans);
	auto hierComp = registry.try_get<HierarchyComp>(entity);
	if (hierComp && hierComp->getParent() != entt::null) {
		TransformComp* parentTrans = registry.try_get<TransformComp>(hierComp->getParent());
		if (parentTrans) {
			glm::vec3 newLocalPos = parentTrans->inverseTransformMatrix * glm::vec4(worldPoint, 1);
			return newLocalPos;
		}
	}
	return worldPoint;
}

glm::quat TransformSystem::orientationLocalToWorld(entt::registry& registry, TransformComp* trans, glm::quat localOrientation)
{
	auto entity = entt::to_entity(registry, *trans);
	auto hierComp = registry.try_get<HierarchyComp>(entity);
	if (hierComp && hierComp->getParent() != entt::null) {
		TransformComp* parentTrans = registry.try_get<TransformComp>(hierComp->getParent());
		if (parentTrans) {
			return parentTrans->worldOrientation * localOrientation;
		}
	}
	return trans->localOrientation;
}

glm::quat TransformSystem::orientationWorldToLocal(entt::registry& registry, TransformComp* trans, glm::quat worldOrientation)
{
	auto entity = entt::to_entity(registry, *trans);
	auto hierComp = registry.try_get<HierarchyComp>(entity);
	if (hierComp && hierComp->getParent() != entt::null) {
		TransformComp* parentTrans = registry.try_get<TransformComp>(hierComp->getParent());
		if (parentTrans) {
			return parentTrans->worldOrientationInverse * worldOrientation;
		}
	}
	return worldOrientation;
}

