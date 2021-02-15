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

void Model::drawNow(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->drawNow();
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
	
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        Mesh *m = new Mesh();
        processMesh(mesh, scene, m->vertices, m->indices);
        m->setupMesh();
        meshes.push_back(m);
		
		std::cout << "Meshes size: " << meshes.size() << std::endl;			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}  

void Model::loadModel(std::string path)
{
    //
	Assimp::Importer import;
    const aiScene *scene = import.ReadFile((QApplication::applicationDirPath() + "/resources/models/").toStdString() + path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
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
    processNode(scene->mRootNode, scene);
}  

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
{
    //std::vector<Texture> textures;
    qDebug() << "Vertices " << mesh->mNumVertices;
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
    qDebug() << "Triangles " << mesh->mNumFaces;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}  
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        
    }

    return new Mesh(vertices, indices);

}  

