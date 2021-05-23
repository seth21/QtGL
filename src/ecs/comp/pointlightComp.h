#ifndef POINTLIGHTCOMP_H
#define POINTLIGHTCOMP_H
#include <memory>
#include <glm/mat4x4.hpp>
class PointLight;

struct PointLightComp {
	std::unique_ptr<PointLight> pointLight;
	glm::mat4 sphereTRS;
	
	std::vector<entt::entity> affectedEntities;
	PointLightComp() {
		pointLight = std::make_unique<PointLight>();
		sphereTRS = glm::mat4(1);
	}

	void setDirty(bool d) {
		lightDirty = d;
	}

	bool isDirty() {
		return lightDirty;
	}
private:
	bool lightDirty = true;
};

#endif