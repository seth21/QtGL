#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>
#include "texture.h"
#include <string>
#include <include/assimp/scene.h>
#include "textureresource.h"
#include "materialresource.h"
#include "modeltexturedef.h"
#include "material.h"


class Resources {
public:
	static Resources& getInstance()
	{
		static Resources instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
	Resources(Resources const&) = delete;
	void operator=(Resources const&) = delete;

	//Path relative to the resources folder!
	void loadTexture(std::string resourcePath);
	void releaseTexture(std::string resourcePath);
	Texture* getTexture(std::string resourcePath);
	
	void releaseMaterial(std::string resourcePath, int id);
	Material* getMaterial(std::string resourcePath, int id);
	void loadMaterialsFromModel(const aiScene* scene, std::string modelResourcePath);

	void loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName, std::string modelResPath, std::vector<ModelTextureDef> &outVector);

private:
	std::vector<TextureResource> textures;
	std::vector<MaterialResource> materials;
	Resources() {
		
	}
};
#endif // !RESOURCES_H
