#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <memory>
#include "resourcemanager.h"
#include "modeltexturedef.h"
#include "texture.h"
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>
#include "shaderprogram.h"

class Material {
public:
	std::vector<std::shared_ptr<Texture>> diffuseMaps;
	std::vector<std::shared_ptr<Texture>> normalMaps;
	std::vector<std::shared_ptr<Texture>> specularMaps;
	std::string modelResourcePath;
	int materialID = 0;
	Material();
	~Material();
	std::vector<ModelTextureDef> diffuseDefs;
	std::vector<ModelTextureDef> specularDefs;
	std::vector<ModelTextureDef> normalDefs;
	std::vector<ModelTextureDef> bumpDefs;
	void releaseAllTextures();
	//Call when all the texture definitions are set
	void fetchTextures();
	void setFloat(std::string name, float x);
	void setVec3(std::string name, glm::vec3 xyz);
	void setTexture(std::string name, std::shared_ptr<Texture> texture);
	std::shared_ptr<ShaderProgram> shader;
	std::unordered_map<std::string, std::shared_ptr<Texture>>& getTextures();
	std::unordered_map<std::string, glm::vec3>& getVec3s();
	std::unordered_map<std::string, float>& getFloats();
private:
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, glm::vec3> vec3s;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};

#endif