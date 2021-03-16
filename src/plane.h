#ifndef PLANE_H
#define PLANE_H
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp> 

enum PlaneSide {
	OnPlane, Back, Front
};

enum PlaneTest {
	Outside, Inside, Intersect
};

class Plane {
public:
	glm::vec3 normal;
	float d = 0;
	Plane();
	Plane(glm::vec3 n, float d);
	Plane(glm::vec3 n, glm::vec3 point);
	Plane(glm::vec3 &point0, glm::vec3 &point1, glm::vec3 &point2);
	float distance(glm::vec3 &point);
	PlaneSide testPoint(glm::vec3 &point);
	void set3Points(glm::vec3 &point0, glm::vec3 &point1, glm::vec3 &point2);
};

#endif