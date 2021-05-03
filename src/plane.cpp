#include "plane.h"

Plane::Plane()
{
	
}

Plane::Plane(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2)
{
	set3Points(p0, p1, p2);
}

Plane::Plane(glm::vec3 n, glm::vec3 point)
{
	normal = glm::normalize(n);
	d = -glm::dot(point, normal);
}

Plane::Plane(glm::vec3 n, float dist)
{
	normal = glm::normalize(n);
	d = dist;
}

void Plane::set3Points(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2)
{
	glm::vec3 v = p1 - p0;
	glm::vec3 u = p2 - p0;
	normal = glm::normalize(glm::cross(v, u));

	d = -glm::dot(p0, normal);
}

PlaneSide Plane::testPoint(const glm::vec3 &point) const
{
	float dist = distance(point);

	if (dist == 0)
		return OnPlane;
	else if (dist < 0)
		return Back;
	else
		return Front;
}


float Plane::distance(const glm::vec3 &point) const{
	return glm::dot(normal, point) + d;
}

