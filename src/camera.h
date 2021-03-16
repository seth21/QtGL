#ifndef CAMERA_H
#define CAMERA_H

#include <QDebug>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <memory>
#include "inputhandler.h"
#include "inputmgr.h"
#include "action.h"
#include "camerafrustum.h"

class Camera : public InputHandler {
public:
	Camera();
	glm::mat4 getPVmatrix();

	glm::mat4 getProjMatrix();

	glm::mat4 getViewMatrix();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation;

	virtual void handleAction(ACTION action, bool pressed) override;
	virtual void handleMouseDelta(int dx, int dy) override;
	void updateVectors();
	const glm::vec3& getFront();
	const glm::vec3& getRight();
	void update(float deltaTime);
	
	std::unique_ptr<CameraFrustum> frustum;
private:
	float rightAngle = 0, upAngle = 0;
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
	float FOVy;
	float nearPlane;
	float farPlane;
	float aspect;
};


#endif // CAMERA

