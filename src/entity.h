#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"

class Entity {
public:
	Entity();
	std::vector<Entity> children;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	std::vector<Mesh*> meshes;
};