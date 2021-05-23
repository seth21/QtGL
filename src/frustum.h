#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "plane.h"
#include <glm/vec3.hpp>
#include <memory>
#include "boundingbox.h"
#include "boundingsphere.h"
#include <QDebug>
class Frustum {
public:
	Frustum();
	~Frustum();
	void calculateFrustum(const float fovYdegrees, const float near, const float far, const float aspect, const glm::vec3& camPos, const glm::vec3& camFront, const glm::vec3& camRight);
	BoundingBox getFrustumAABB();
	bool intersects();
	std::unique_ptr<Plane> planes[6];
	bool pointInFrustum(glm::vec3& p);
	bool sphereInFrustum(const glm::vec3& p, float radius);
	bool sphereInFrustum(const BoundingSphere &sphere);
	bool boundsInFrustum(const BoundingBox& bounds);
	std::vector<glm::vec3> getLineSegments();
	const std::vector<glm::vec3>& getFrustumPoints();
private:
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};
	std::vector<glm::vec3> frustumPoints;
};

#endif