#ifndef TRANSFORMCOMP_H
#define TRANSFORMCOMP_H
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "boundingbox.h"
#include "boundingsphere.h"
#include <entt/entt.hpp>

struct TransformComp {

	TransformComp() {
		position = glm::vec3(0);
		scale = glm::vec3(1);
		orientation = glm::quat(1.0, 0.0, 0.0, 0.0);
		transformMatrix = glm::mat4(1);
		dirty = true;
	}

	glm::vec3 getPosition() {
		return position;
	}

	glm::vec3 getScale() {
		return scale;
	}

	glm::quat getOrientation() {
		return orientation;
	}

	glm::mat4 getWorldMatrix() {
		return transformMatrix;
	}

	void setWorldMatrix(glm::mat4 mat) {
		transformMatrix = mat;
	}

	void setPosition(glm::vec3 pos) {
		position = pos;
		dirty = true;
	}

	void setScale(glm::vec3 scl) {
		scale = scl;
		dirty = true;
	}

	void setOrientation(glm::quat quat) {
		orientation = quat;
		dirty = true;
	}

	bool isDirty() {
		return dirty;
	}

	void setDirty(bool d) {
		dirty = d;
	}

	BoundingBox& getBoundingBox() {
		return boundingBox;
	}

	BoundingSphere& getBoundingSphere() {
		return boundingSphere;
	}

	std::vector<entt::entity>& getChildren() {
		return children;
	}

	//entt::entity first_child{ entt::null };
	//entt::entity prev_sibling{ entt::null };
	//entt::entity next_sibling{ entt::null };
	entt::entity parent{ entt::null };
	unsigned char hierarchyDepth = 0;


private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	glm::mat4 transformMatrix;
	bool dirty;
	BoundingBox boundingBox;
	BoundingSphere boundingSphere;
	std::vector<entt::entity> children;
	void dirtyHierarchy() {
		dirty = true;
		for (auto child : children) {
			//m_ecs
		}
	}

};
#endif