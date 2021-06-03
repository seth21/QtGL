#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
#include "componentSystem.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/hierarchyComp.h"
#include <glm/gtc/matrix_transform.hpp>


class TransformSystem : public ComponentSystem {
public:
	TransformSystem(entt::registry* registry, MasterRenderer* masterRenderer);
	void update(float deltaTime);
	void transformChanged(entt::registry& registry, entt::entity e);
	void transformAdded(entt::registry& registry, entt::entity e);
	void transformRemoved(entt::registry& registry, entt::entity e);
	void dirtyAdded(entt::registry& registry, entt::entity e);
	bool first = false;
	static glm::vec3 getWorldPosition(entt::registry& registry, TransformComp* trans);
	static void setWorldPosition(entt::registry& registry, TransformComp* trans, glm::vec3 worldPos);
	static glm::quat getWorldOrientation(entt::registry& registry, TransformComp* trans);
	static void setWorldOrientation(entt::registry &registry, TransformComp* trans, glm::quat worldPos);
	static glm::vec3 getWorldScale(TransformComp* trans);
	//Utility methods
	static void rotate(entt::registry& registry, TransformComp* trans, glm::vec3 eulerDegrees, bool relativeToSelf = true);
	static void rotateAround(entt::registry& registry, TransformComp* trans, glm::vec3 point, glm::vec3 axis, float angleDegrees);
	static void translate(entt::registry& registry, TransformComp* trans, glm::vec3 translation, bool relativeToSelf = true);
	static void lookAt(entt::registry& registry, TransformComp* trans, glm::vec3 normalizedDirection, glm::vec3 worldUp = glm::vec3(0,1,0));
	static glm::vec3 getForward(entt::registry& registry, TransformComp* trans);
	//Space conversion methods
	static glm::vec3 pointLocalToWorld(entt::registry& registry, TransformComp* trans, glm::vec3 localPoint);
	static glm::vec3 pointWorldToLocal(entt::registry& registry, TransformComp* trans, glm::vec3 worldPoint);
	static glm::quat orientationLocalToWorld(entt::registry& registry, TransformComp* trans, glm::quat localOrientation);
	static glm::quat orientationWorldToLocal(entt::registry& registry, TransformComp* trans, glm::quat worldOrientation);
};
#endif