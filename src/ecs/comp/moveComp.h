#ifndef MOVECOMP_H
#define MOVECOMP_H
#include <memory>
#include <glm/vec3.hpp>

struct MoveComp {

	MoveComp() {
		velocity = glm::vec3(0, 0, 0);
		velRelativeToSelf = true;
		angularVelocity = glm::vec3(0, 0, 0);
		angularVelRelativeToSelf = true;
		clearAfterMove = true;
		scale = glm::vec3(0);
	}
	glm::vec3 scale;
	glm::vec3 velocity;
	bool velRelativeToSelf;
	glm::vec3 angularVelocity;
	bool angularVelRelativeToSelf;
	bool clearAfterMove;

};

#endif