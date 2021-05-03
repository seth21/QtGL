#ifndef SHADERRESOLVER_H
#define SHADERRESOLVER_H
#include "material.h"

struct ShaderVariant {
	
};

class ShaderResolver {
public:
	ShaderResolver();
	void assignShaderToMaterial(Material* mat);
private:
	std::string ALBEDO_DEFINE = "ALBEDO";
	std::string NORMAL_DEFINE = "NORMAL";
	std::string SPECULAR_DEFINE = "SPECULAR";
	std::string BUMP_DEFINE = "BUMP";
};

#endif