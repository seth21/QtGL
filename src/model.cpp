#include "model.h"


Model::Model(std::string fileName){
    initializeOpenGLFunctions();
    loadModel(fileName);
}

Model::~Model(){
    for (int i = 0; i < meshes.size(); i++) {
        delete meshes[i];
    }
}

void Model::addMesh(Mesh mesh){

}



void Model::processNode(aiNode *node, const aiScene *scene, Entity *e)
{
    //get the transformation details
    aiVector3D pos;
    aiVector3D scale;
    aiQuaternion orientation;
    node->mTransformation.Decompose(pos, orientation, scale);
    e->position.x = pos.x;
    e->position.y = pos.y;
    e->position.z = pos.z;
    e->scale.x = scale.x == 0 ? 1 : scale.x;
    e->scale.y = scale.y == 0 ? 1 : scale.y;
    e->scale.z = scale.z == 0 ? 1 : scale.z;
    e->rotation.x = orientation.x;
    e->rotation.y = orientation.y;
    e->rotation.z = orientation.z;
    e->rotation.w = orientation.w;
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        Mesh *m = new Mesh();
        processMesh(mesh, scene, m->vertices, m->indices);
        m->setupMesh();
        //meshes.push_back(m);
        e->meshes.push_back(m);
        Material* meshMat = Resources::getInstance().getMaterial(filepath, mesh->mMaterialIndex);
        e->materials.push_back(meshMat);
		//std::cout << "Meshes size: " << meshes.size() << std::endl;			
    }//mesh->mMaterialIndex
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Entity *echild = new Entity();
        echild->parent = e;
        processNode(node->mChildren[i], scene, echild);
        e->children.push_back(echild);
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
    Resources::getInstance().loadMaterialsFromModel(scene, path);
    entity = new Entity();
    processNode(scene->mRootNode, scene, entity);

}  

void Model::processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
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
    // Process materials
	if (mesh->mMaterialIndex >= 0)
	{
        /*qDebug() << "Found Materials";
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'tex_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// eg: tex_diffuseN, tex_specularN, tex_normalN

		// 1. Diffuse maps
		std::vector<ModelTextureDef> diffuseMaps = loadMaterialTextureDefs(material, aiTextureType_DIFFUSE, "tex_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		std::vector<ModelTextureDef> specularMaps = loadMaterialTextureDefs(material, aiTextureType_SPECULAR, "tex_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. Normal maps
		std::vector<ModelTextureDef> normalMaps = loadMaterialTextureDefs(material, aiTextureType_NORMALS, "tex_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());*/

	}

}  

/*std::vector<ModelTextureDef> Model::loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    //qDebug() << "loadMat" << mat->GetTextureCount
    std::string dir = filepath;
    auto pos = dir.rfind('/');
    if (pos != std::string::npos) {
        dir.erase(pos);
        dir += '/';
    }
    else dir = "";
    std::vector<ModelTextureDef> textures;

    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        ModelTextureDef textureDef;
        textureDef.id = i;
        textureDef.type = typeName;
        textureDef.path = dir + str.C_Str();
        textures.push_back(textureDef);
        qDebug() << textureDef.type.c_str() << ","<< textureDef.path.c_str();
    }
    return textures;
}*/
