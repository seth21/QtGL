#include "camera.h"

Camera::Camera()
{
	rotation = glm::quatLookAt(forward, up);
	InputManager::getInstance().registerHandler(this);
	position = glm::vec3(0, 0, 35);
}

glm::mat4 Camera::getPVmatrix()
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.f);
	
	
	glm::mat4 rot = glm::toMat4(rotation);
	glm::mat4 translation = glm::translate(rot, -position);

	
	return Projection * (translation * rot);
}

void Camera::handleAction(ACTION action, bool pressed)
{
	
}

void Camera::handleMouseDelta(int dx, int dy)
{
	pitch += dy;
	yaw += dx;
	glm::quat rotUp = glm::angleAxis(yaw * 0.005f, up);
	glm::quat rotRight = glm::angleAxis(pitch * 0.005f, right);
	rotation = rotRight * rotUp;
	rotation = glm::normalize(rotation);

}

void Camera::update(float deltaTime)
{
	
	//Update vectors
	frontC = glm::rotate(glm::inverse(rotation), forward);
	rightC = glm::rotate(glm::inverse(rotation), right);
	upC = glm::vec3(0.0, 1.0, 0.0);

	//Handle Input
	if (InputManager::getInstance().isPressed(FORWARD)) {
		position += frontC * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(BACK)) {
		position -= frontC * 5.0f * deltaTime;
	}
	if (InputManager::getInstance().isPressed(LEFT)) {
		position -= rightC * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(RIGHT)) {
		position += rightC * 5.0f * deltaTime;
	}
	if (InputManager::getInstance().isPressed(UP)) {
		position += upC * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(DOWN)) {
		position -= upC * 5.0f * deltaTime;
	}
}
