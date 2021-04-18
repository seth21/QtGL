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
		//if (tex) {diffuseMaps.push_back(tex);
		if (tex) textures["albedoMap"] = tex;
	}
	for (int i = 0; i < normalDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(normalDefs[i].path);
		//if (tex) normalMaps.push_back(tex);
		if (tex) textures["normalMap"] = tex;
	}
	for (int i = 0; i < specularDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(specularDefs[i].path);
		//if (tex) specularMaps.push_back(tex);
		if (tex) textures["specularMap"] = tex;
	}
	for (int i = 0; i < bumpDefs.size(); i++) {
		std::shared_ptr<Texture> tex = ResourceManager::getInstance().load<Texture>(bumpDefs[i].path);
		//if (tex) specularMaps.push_back(tex);
		if (tex) textures["bumpMap"] = tex;
	}
}

void Material::setFloat(std::string name, float x)
{
	floats[name] = x;
}

void Material::setVec3(std::string name, glm::vec3 xyz)
{
	vec3s[name] = xyz;
}

void Material::setTexture(std::string name, std::shared_ptr<Texture> texture)
{
	textures[name] = texture;
}

void Material::setTempTexture(std::string name, Texture* texture)
{
	temptextures[name] = texture;
}

std::unordered_map<std::string, std::shared_ptr<Texture>>& Material::getTextures()
{
	return textures;
}

std::unordered_map<std::string, Texture*>& Material::getTempTextures()
{
	return temptextures;
}

std::unordered_map<std::string, glm::vec3>& Material::getVec3s()
{
	return vec3s;
}

std::unordered_map<std::string, float>& Material::getFloats()
{
	return floats;
}

void Material::releaseAllTextures()
{
}
