#ifndef SHADERRESOLVER_H
#define SHADERRESOLVER_H
#include "material.h"

struct ShaderVariant {
	
};

class ShaderResolver {
public:
	//ShaderResolver();
	static void assignShaderToMaterial(Material* mat);
private:
	//standard defines
	const static inline std::string ALBEDO_DEFINE = "ALBEDO";
	const static inline std::string NORMAL_DEFINE = "NORMAL";
	const static inline std::string SPECULAR_DEFINE = "SPECULAR";
	const static inline std::string BUMP_DEFINE = "BUMP";
	const static inline std::string ALPHATEST_DEFINE = "ALPHATEST";
	//standard shaders
	const static inline std::string STANDARD_DEFERRED_SHADER = "gbuffer";
	const static inline std::string STANDARD_FORWARD_SHADER = "forward";
};

#endif