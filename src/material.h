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
#include <glm/mat4x4.hpp>
#include "shaderprogram.h"
#include "utils/flags.h"

enum class MaterialFlag {
	TWOSIDED,
	CASTSHADOW,
	RECEIVESHADOW,
	CUSTOMSHADER,
	COUNT
};

enum class RenderPath {
	DEFERRED,
	FORWARD_OPAQUE,
	FORWARD_TRANSPARENT,
	COUNT
};

enum class RenderBlend {
	STANDARD,
	ALPHATEST,
	ADDITIVE,
	NONE,
	COUNT
};

class Material {
public:

	std::string modelResourcePath;
	int materialID = 0;
	std::string internalName;
	float shininess;
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
	void setVec2(std::string name, glm::vec2 xy);
	void setVec3(std::string name, glm::vec3 xyz);
	void setTexture(std::string name, std::shared_ptr<Texture> texture);
	void setTempTexture(std::string name, Texture *texture);
	void setMat4(std::string name, glm::mat4x4 mat4);

	std::shared_ptr<ShaderProgram> shader;
	std::unordered_map<std::string, std::shared_ptr<Texture>>& getTextures();
	std::unordered_map<std::string, Texture*>& getTempTextures();
	std::unordered_map<std::string, glm::vec3>& getVec3s();
	std::unordered_map<std::string, glm::vec2>& getVec2s();
	std::unordered_map<std::string, glm::mat4x4>& getMat4s();
	std::unordered_map<std::string, float>& getFloats();
	Flags<MaterialFlag> state;

	const static std::string ALBEDO_UNIFORM, NORMAL_UNIFORM, SPEC_UNIFORM, BUMP_UNIFORM;
	bool textureUniformExists(std::string name);
	bool hasAlbedoWithAlpha();
	RenderBlend blending = RenderBlend::NONE;
	RenderPath renderPath = RenderPath::DEFERRED;
private:
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, glm::vec2> vec2s;
	std::unordered_map<std::string, glm::vec3> vec3s;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	std::unordered_map<std::string, glm::mat4x4> mat4s;
	std::unordered_map<std::string, Texture*> temptextures;
	
};

#endif