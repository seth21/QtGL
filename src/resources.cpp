#include "resources.h"


void Resources::loadTexture(std::string resourcePath)
{
	//Preload a texture
	Texture *texture = new Texture(resourcePath);
	if (!texture->fileLoaded()) {
		qDebug() << "Resource not found:" << resourcePath.c_str();
		delete texture;
		return;
	}
	TextureResource texRes;
	texRes.texture = texture;
	texRes.resourcePath = resourcePath;
	texRes.references = 0;
	textures.push_back(texRes);

}

//When references go down to zero, release texture opengl resources
//Each call to getTexture +1 reference, to releaseTexture -1 reference
void Resources::releaseTexture(std::string resourcePath)
{
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].resourcePath == resourcePath) {
			textures[i].references--;
			if (textures[i].references <= 0) {
				Texture* tex = textures[i].texture;
				//release gl resources
				tex->release();
				//release texture object
				delete tex;
				//remove from vector
				textures.erase(textures.begin() + i);
			}
			return;
		}
	}

}

Texture* Resources::getTexture(std::string resourcePath)
{
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].resourcePath == resourcePath) {
			textures[i].references++;
			return textures[i].texture;
		}
	}
	//Not found, load it now
	loadTexture(resourcePath);
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].resourcePath == resourcePath) {
			textures[i].references++;
			return textures[i].texture;
		}
	}
	return NULL;
}

void Resources::releaseMaterial(std::string resourcePath, int id)
{
	//release all associated textures
}

Material* Resources::getMaterial(std::string resourcePath, int id)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].resourcePath == resourcePath && materials[i].materialID == id) {
			materials[i].references++;
			//qDebug() << "Found material:" << resourcePath.c_str() << " ID:" << id << "References:" << materials[i].references;
			return materials[i].material;
		}
	}
	qDebug() << "ERROR - NOT Found material!!!:" << resourcePath.c_str() << " ID:" << id;
	return nullptr;
}

void Resources::loadMaterialsFromModel(const aiScene* scene, std::string modelResourcePath)
{
	for (int i = 0; i < scene->mNumMaterials; i++) {
		//qDebug() << "Found Materials";
		aiMaterial* material = scene->mMaterials[i];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'tex_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// eg: tex_diffuseN, tex_specularN, tex_normalN
		Material *mat = new Material();
		mat->materialID = i;
		mat->modelResourcePath = modelResourcePath;
		// 1. Diffuse maps
		loadMaterialTextureDefs(material, aiTextureType_DIFFUSE, "tex_diffuse", modelResourcePath, mat->diffuseDefs);
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		loadMaterialTextureDefs(material, aiTextureType_SPECULAR, "tex_specular", modelResourcePath, mat->specularDefs);
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. Normal maps
		loadMaterialTextureDefs(material, aiTextureType_NORMALS, "tex_normal", modelResourcePath, mat->normalDefs);
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());*/
		mat->fetchTextures();
		MaterialResource matResource;
		matResource.material = mat;
		matResource.materialID = i;
		matResource.resourcePath = modelResourcePath;
		matResource.references = 0;
		materials.push_back(matResource);
		
	}
	
}

void Resources::loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName, std::string modelResPath, std::vector<ModelTextureDef> &outVector)
{
	//qDebug() << "loadMat" << mat->GetTextureCount
	std::string dir = modelResPath;
	auto pos = dir.rfind('/');
	if (pos != std::string::npos) {
		dir.erase(pos);
		dir += '/';
	}
	else dir = "";
	//std::vector<ModelTextureDef> textures;

	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = str.C_Str();
		std::replace(path.begin(), path.end(), '\\', '/');
		ModelTextureDef textureDef;
		textureDef.id = i;
		textureDef.type = typeName;
		textureDef.path = dir + path;
		outVector.push_back(textureDef);
		//textures.push_back(textureDef);
		//qDebug() << textureDef.type.c_str() << "," << textureDef.path.c_str();
	}
}
