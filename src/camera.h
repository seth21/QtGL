#ifndef CAMERA_H
#define CAMERA_H

#include <QDebug>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "inputhandler.h"
#include "inputmgr.h"
#include "action.h"

class Camera : public InputHandler {
public:
	Camera();
	glm::mat4 getPVmatrix();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation;

	//defaults
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	//camera vectors
	glm::vec3 frontC = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 rightC = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 upC = glm::vec3(0.0f, 1.0f, 0.0f);

	virtual void handleAction(ACTION action, bool pressed) override;
	virtual void handleMouseDelta(int dx, int dy) override;
	void update(float deltaTime);
private:
	int pitch = 0, yaw = 0;

};


#endif // CAMERA

