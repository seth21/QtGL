#include "shaderresolver.h"

void ShaderResolver::assignShaderToMaterial(Material* mat)
{
	if (mat->state[MaterialFlag::CUSTOMSHADER]) return; //No automatic assignment.

	if (mat->renderPath == RenderPath::DEFERRED) {
		ResourceConfig config;
		if (mat->textureUniformExists(Material::ALBEDO_UNIFORM)) config.addFlag(ALBEDO_DEFINE);
		if (mat->textureUniformExists(Material::NORMAL_UNIFORM)) config.addFlag(NORMAL_DEFINE);
		if (mat->textureUniformExists(Material::SPEC_UNIFORM)) config.addFlag(SPECULAR_DEFINE);
		if (mat->textureUniformExists(Material::BUMP_UNIFORM)) config.addFlag(BUMP_DEFINE);
		if (mat->hasAlbedoWithAlpha()) {
			mat->blending == RenderBlend::ALPHATEST;
			config.addFlag(ALPHATEST_DEFINE);
		}

		mat->shader = ResourceManager::getInstance().load<ShaderProgram>(STANDARD_DEFERRED_SHADER, config);
	}
	else if (mat->renderPath == RenderPath::FORWARD_OPAQUE || mat->renderPath == RenderPath::FORWARD_TRANSPARENT) {
		//Not implemented yet.
	}
}
