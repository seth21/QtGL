#ifndef PLAYERINPUTCOMP_H
#define PLAYERINPUTCOMP_H

struct PlayerInputComp {
	
	PlayerInputComp() {
		velocity = glm::vec3(0);
	}
	glm::vec3 velocity;
};

#endif