#include "material.h"
#include "resources.h"

Material::Material()
{
}

void Material::fetchTextures()
{
	for (int i = 0; i < diffuseDefs.size(); i++) {
		Texture *tex = Resources::getInstance().getTexture(diffuseDefs[i].path);
		if (tex) diffuseMaps.push_back(tex);
	}
	for (int i = 0; i < normalDefs.size(); i++) {
		Texture* tex = Resources::getInstance().getTexture(normalDefs[i].path);
		if (tex) normalMaps.push_back(tex);
	}
	for (int i = 0; i < specularDefs.size(); i++) {
		Texture* tex = Resources::getInstance().getTexture(specularDefs[i].path);
		if (tex) specularMaps.push_back(tex);
	}
}

void Material::releaseAllTextures()
{
}
