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
#include "entity.h"
#include "resources.h"

class Model : protected QOpenGLExtraFunctions{

    public:
        Model(std::string fileName);
        ~Model();
        void addMesh(Mesh mesh);
        
        std::vector<Mesh*> meshes;
        void loadModel(std::string path);
        Entity* entity;
        std::string filepath;
    private:
        std::vector<ModelTextureDef> textures;
        void processNode(aiNode *node, const aiScene *scene, Entity *e);
        void processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
        std::vector<ModelTextureDef> loadMaterialTextureDefs(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif