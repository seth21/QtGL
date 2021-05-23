#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "boundingbox.h"
class BoundingSphere {

public:
	BoundingSphere(glm::vec3 center, float radius);
	BoundingSphere();
	~BoundingSphere();
	void set(glm::vec3 center, float radius);
	bool intersect(const BoundingSphere& sphere);
	void calculateSphereApproximate(BoundingBox& aabb);
	float surfaceArea();
	glm::vec3 m_center = glm::vec3(0.0f);
	float m_radius = 1;
private:
	float PI = 3.1415f;
	
};

#endif