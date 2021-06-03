#ifndef TRANSFORMCOMP_H
#define TRANSFORMCOMP_H
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "boundingbox.h"
#include "boundingsphere.h"
#include <entt/entt.hpp>
#include <ecs/comp/dirtytransformComp.h>

struct TransformComp {

	glm::vec3 localPosition;
	glm::vec3 localScale;
	glm::quat localOrientation;
	glm::quat worldOrientation;
	glm::quat worldOrientationInverse;
	glm::vec3 worldScale;
	glm::mat4 transformMatrix;
	glm::mat4 inverseTransformMatrix;
	//glm::mat4 parentInverse;
	//glm::quat parentOrientationInverse;
	TransformComp() {
		localPosition = glm::vec3(0);
		localScale = glm::vec3(1);
		localOrientation = glm::quat(1.0, 0.0, 0.0, 0.0);
		transformMatrix = glm::mat4(1);
		//dirty = true;
	}

	BoundingBox& getBoundingBox() {
		return boundingBox;
	}

	BoundingSphere& getBoundingSphere() {
		return boundingSphere;
	}

private:
	BoundingBox boundingBox;
	BoundingSphere boundingSphere;
	
	

};
#endif