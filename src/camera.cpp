#include "camera.h"

Camera::Camera()
{
	rotation = glm::quat(0, 0, 0, -1);
	InputManager::getInstance().registerHandler(this);
	position = glm::vec3(0, 0, 35);
	updateVectors();
}

glm::mat4 Camera::getPVmatrix()
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);	
	return Projection * getViewMatrix();
}

glm::mat4 Camera::getProjMatrix() {
	return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);
}

glm::mat4 Camera::getViewMatrix()
{
	// You should know the camera move reversely relative to the user input.

	glm::quat reverseOrient = glm::conjugate(rotation);
	glm::mat4 rot = glm::mat4_cast(reverseOrient);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -position);
	return rot * translation;
}

void Camera::handleAction(ACTION action, bool pressed)
{
	
}

void Camera::handleMouseDelta(int dx, int dy)
{
	rightAngle += dx * 0.4f;
	upAngle += dy * 0.3f;
	float pitchLimit = 35.0f;
	if (upAngle > pitchLimit) {
		upAngle = pitchLimit;
	}
	else if (upAngle < -pitchLimit) {
		upAngle = -pitchLimit;
	}
	updateVectors();

}

void Camera::updateVectors() {
	// Yaw
	glm::quat aroundY = glm::angleAxis(glm::radians(-rightAngle), glm::vec3(0, 1, 0));

	// Pitch
	glm::quat aroundX = glm::angleAxis(glm::radians(upAngle), glm::vec3(1, 0, 0));

	rotation = aroundY * aroundX;
}

const glm::vec3& Camera::getFront() {
	return Front;
}

const glm::vec3& Camera::getRight() {
	return Right;
}

void Camera::update(float deltaTime)
{
	
	//Get vectors
	glm::quat qF = rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(rotation);
	Front = { qF.x, qF.y, qF.z };
	Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));

	//Handle Input
	if (InputManager::getInstance().isPressed(FORWARD)) {
		position += Front * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(BACK)) {
		position -= Front * 5.0f * deltaTime;
	}
	if (InputManager::getInstance().isPressed(LEFT)) {
		position -= Right * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(RIGHT)) {
		position += Right * 5.0f * deltaTime;
	}
	if (InputManager::getInstance().isPressed(UP)) {
		position += glm::vec3(0, 1, 0) * 5.0f * deltaTime;
	}
	else if (InputManager::getInstance().isPressed(DOWN)) {
		position -= glm::vec3(0, 1, 0) * 5.0f * deltaTime;
	}
}
