#ifndef CAMERAFRUSTUM_H
#define CAMERAFRUSTUM

#include "plane.h"
#include <glm/vec3.hpp>
#include <memory>
#include "boundingbox.h"
#include <QDebug>
class CameraFrustum {
public:
	CameraFrustum();
	~CameraFrustum();
	void calculateFrustum(const float fovYdegrees, const float near, const float far, const float aspect, const glm::vec3& camPos, const glm::vec3& camFront, const glm::vec3& camRight);
	bool intersects();
	std::unique_ptr<Plane> planes[6];
	bool pointInFrustum(glm::vec3& p);
	bool sphereInFrustum(glm::vec3& p, float radius);
	bool CameraFrustum::boundsInFrustum(BoundingBox& bounds);
private:
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};
};

#endif