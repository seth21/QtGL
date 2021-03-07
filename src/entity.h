#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include "shaderprogram.h"
#include "texture.h"
#include "model.h"
#include <memory>

class Entity {
public:
	Entity();
	Entity* parent = NULL;
	std::vector<Entity*> children;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 trsMatrix;
	std::shared_ptr<Model> model;
	
	void drawNow(ShaderProgram &shader);
	void updateChildrenMatrices();
	glm::mat4 getTRSMatrix();
};