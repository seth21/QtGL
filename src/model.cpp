#include "model.h"


Model::Model(std::string fileName, ResourceConfig config) : Resource (fileName, config){

    loadModel(fileName);
}

Model::~Model(){
    for (int i = 0; i < meshes.size(); i++) {
        delete meshes[i];
    }
    for (int i = 0; i < materials.size(); i++) {
        delete materials[i];
    }

}

void Model::addMesh(Mesh mesh){

}

bool Model::loaded()
{
    return fileLoaded;
}



void Model::processNodes(aiNode *node, const aiScene *scene, MeshNode* meshnode)
{
    //get the transformation details
    aiVector3D pos;
    aiVector3D scale;
    aiQuaternion orientation;
    node->mTransformation.Decompose(scale, orientation, pos);
    meshnode->position = glm::vec3(pos.x, pos.y, pos.z);
    meshnode->scale = glm::vec3(scale.x, scale.y, scale.z);
    meshnode->orientation = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
    meshnode->name = node->mName.C_Str();
    // add all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        meshnode->meshIndices.push_back(node->mMeshes[i]); 
		//std::cout << "Meshes size: " << meshes.size() << std::endl;			
    }
    //qDebug() << node->mName.C_Str() << " Meshes:" << node->mNumMeshes << " Children:" << node->mNumChildren << " Parent:" << (node->mParent ? node->mParent->mName.C_Str() : "NULL");
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        std::unique_ptr child = std::make_unique<MeshNode>();
        processNodes(node->mChildren[i], scene, child.get());
        meshnode->children.push_back(std::move(child));
    }
}

void Model::processMeshes(const aiScene* scene)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        Mesh* m = new Mesh();
        loadMeshVertices(mesh, scene, m->vertices, m->indices);
        m->setupMesh();
        m->materialIndex = mesh->mMaterialIndex;
        m->aabb.findMinMax(m->vertices);
        meshes.push_back(m);
    }

}

void Model::loadMaterialsFromModel(const aiScene* scene, std::string modelResourcePath)
{
    for (int i = 0; i < scene->mNumMaterials; i++) {
        //qDebug() << "Found Materials";
        aiMaterial* material = scene->mMaterials[i];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'tex_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // eg: tex_diffuseN, tex_specularN, tex_normalN
        Material* mat = new Material();
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
        // 4. Bump maps
        loadMaterialTextureDefs(material, aiTextureType_HEIGHT, "tex_bump", modelResourcePath, mat->bumpDefs);
        //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());*/
        mat->fetchTextures();
        //other properties
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        mat->internalName = name.C_Str();
        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);
        mat->shininess = shininess;
        aiColor3D specularColor;
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        mat->setVec3("specularColor", glm::vec3(specularColor.r, specularColor.g, specularColor.b));
        //qDebug() << mat->internalName.c_str() << " s " << specularColor.r << " " << specularColor.g;

        materials.push_back(mat);

    }

}

void Model::loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName, std::string modelResPath, std::vector<ModelTextureDef>& outVector)
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

void Model::loadModel(std::string path)
{
    filepath = path;
	Assimp::Importer import;
    const aiScene *scene = import.ReadFile((QApplication::applicationDirPath() + "/resources/").toStdString() + path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString();
        return;
    }
	else{
        qDebug() << "ASSIMP::Loaded model->" << path.c_str();
	}
    //directory = path.substr(0, path.find_last_of('/'));
    qDebug() << "Scene meshes " << scene->mNumMeshes;
    qDebug() << "Scene materials " << scene->mNumMaterials;
    loadMaterialsFromModel(scene, path);
    meshnode = std::make_unique<MeshNode>();
    processNodes(scene->mRootNode, scene, meshnode.get());
    processMeshes(scene);
    fileLoaded = true;
}

ResourceConfig& Model::getResourceConfig()
{
    return config;
}

void Model::loadMeshVertices(aiMesh *mesh, const aiScene *scene, std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
{
   
    //std::vector<Texture> textures;
    //qDebug() << "Vertices " << mesh->mNumVertices;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
		glm::vec3 vector; 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z; 
		vertex.Position = vector; 

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;  

        if (mesh->HasTangentsAndBitangents()) {
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
        }
        
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
    // process indices
    //qDebug() << "Triangles " << mesh->mNumFaces;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}  
 

}