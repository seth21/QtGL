#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <QDir>
#include <QApplication>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include "resource.h"
#include "resourceconfig.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct MeshNode {
    std::vector<unsigned int> meshIndices;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat orientation;
    std::vector<std::unique_ptr<MeshNode>> children;
    std::string name;
};

class Model : public Resource{

    public:
        Model(std::string fileName, ResourceConfig config);
        ~Model();
        void addMesh(Mesh mesh);
        bool loaded();
        std::vector<Mesh*> meshes;
        std::vector<Material*> materials;
        std::unique_ptr<MeshNode> meshnode;
        void loadModel(std::string path);
        //Entity* entity;
        std::string filepath;
        ResourceConfig& getResourceConfig();
    private:
        ResourceConfig config;
        bool fileLoaded = false;
        void processNodes(aiNode *node, const aiScene *scene, MeshNode *meshnode);
        void processMeshes(const aiScene* scene);
        void loadMeshVertices(aiMesh *mesh, const aiScene *scene, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
        //std::vector<ModelTextureDef> loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName);
        void loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName, std::string modelResPath, std::vector<ModelTextureDef>& outVector);
        void loadMaterialsFromModel(const aiScene* scene, std::string modelResourcePath);
};

#endif