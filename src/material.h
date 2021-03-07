#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <memory>
#include "resourcemanager.h"
#include "modeltexturedef.h"
#include "texture.h"

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
	void releaseAllTextures();
	//Call when all the texture definitions are set
	void fetchTextures();
};

#endif