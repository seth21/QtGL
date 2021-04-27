#ifndef TRANSFORMCOMP_H
#define TRANSFORMCOMP_H
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformComp {
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	glm::mat4 transformMatrix;
	bool dirty;

	TransformComp() {
		position = glm::vec3(0);
		scale = glm::vec3(1);
		orientation = glm::quat(1.0, 0.0, 0.0, 0.0);
		transformMatrix = glm::mat4(1);
		dirty = true;
	}
};
#endif