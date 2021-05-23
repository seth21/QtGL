#include "boundingsphere.h"

BoundingSphere::BoundingSphere(glm::vec3 center, float radius)
{
	m_center = center;
	m_radius = radius;
}

BoundingSphere::BoundingSphere()
{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::set(glm::vec3 center, float radius)
{
	m_center = center;
	m_radius = radius;
}


bool BoundingSphere::intersect(const BoundingSphere& sphere)
{
	return glm::distance2(m_center, sphere.m_center) < (m_radius + sphere.m_radius) * (m_radius + sphere.m_radius);
}

void BoundingSphere::calculateSphereApproximate(BoundingBox& aabb)
{
	m_center = aabb.getCenter();
	auto &corners = aabb.getCorners();
	float maxDist2 = glm::distance2(m_center, corners[0]);
	float tmpDist2 = 0;
	for (int i = 1; i < corners.size(); i++) {
		if (tmpDist2 = glm::distance2(m_center, corners[i]) > maxDist2) {
			maxDist2 = tmpDist2;
		}
	}
	m_radius = sqrt(maxDist2);
}

float BoundingSphere::surfaceArea()
{
	return 4 * PI * m_radius * m_radius;
}
