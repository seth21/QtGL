#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <glm/vec3.hpp>
class PointLight {
public :
	PointLight();
	glm::vec3 lightColor = glm::vec3(12, 0, 2);
	float constant = 1;
	float linear = 0.7f;
	float quadratic = 1.8f;
	glm::vec3 position = glm::vec3(0, 4, 0);
	float radius = 6;
};
#endif // !POINTLIGHT_H
