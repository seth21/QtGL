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
#include <QOpenGLFunctions>
#include <QDebug>
#include "resource.h"
#include "resourceconfig.h"

class Model : public Resource, protected QOpenGLExtraFunctions{

    public:
        Model(std::string fileName, ResourceConfig config);
        ~Model();
        void addMesh(Mesh mesh);
        bool loaded();
        std::vector<Mesh*> meshes;
        std::vector<Material*> materials;
        void loadModel(std::string path);
        //Entity* entity;
        std::string filepath;
        ResourceConfig& getResourceConfig();
    private:
        ResourceConfig config;
        bool fileLoaded = false;
        //void processNode(aiNode *node, const aiScene *scene);
        void processMeshes(const aiScene* scene);
        void loadMeshVertices(aiMesh *mesh, const aiScene *scene, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
        //std::vector<ModelTextureDef> loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName);
        void loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName, std::string modelResPath, std::vector<ModelTextureDef>& outVector);
        void loadMaterialsFromModel(const aiScene* scene, std::string modelResourcePath);
};

#endif