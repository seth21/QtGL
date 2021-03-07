#include "material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::fetchTextures()
{
	for (int i = 0; i < diffuseDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(diffuseDefs[i].path);
			//Resources::getInstance().getTexture(diffuseDefs[i].path);
		if (tex) diffuseMaps.push_back(tex);
	}
	for (int i = 0; i < normalDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(normalDefs[i].path);
		if (tex) normalMaps.push_back(tex);
	}
	for (int i = 0; i < specularDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(specularDefs[i].path);
		if (tex) specularMaps.push_back(tex);
	}
}

void Material::releaseAllTextures()
{
}
