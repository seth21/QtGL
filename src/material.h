#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

class Texture;
class ModelTextureDef;

class Material {
public:
	std::vector<Texture*> diffuseMaps;
	std::vector<Texture*> normalMaps;
	std::vector<Texture*> specularMaps;
	std::string modelResourcePath;
	int materialID = 0;
	Material();
	std::vector<ModelTextureDef> diffuseDefs;
	std::vector<ModelTextureDef> specularDefs;
	std::vector<ModelTextureDef> normalDefs;
	void releaseAllTextures();
	//Call when all the texture definitions are set
	void fetchTextures();
};

#endif