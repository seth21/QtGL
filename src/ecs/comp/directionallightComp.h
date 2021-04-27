#ifndef DIRECTIONALLIGHTCOMP_H
#define DIRECTIONALLIGHTCOMP_H
#include <memory>
class DirectionalLight;

struct DirectionalLightComp {
	std::unique_ptr<DirectionalLight> dirLight;
	DirectionalLightComp() {
		dirLight = std::make_unique<DirectionalLight>();
	}
};

#endif