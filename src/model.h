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

class Model : protected QOpenGLExtraFunctions{

    public:
        Model(std::string fileName);
        ~Model();
        void addMesh(Mesh mesh);
        void drawNow();
        std::vector<Mesh*> meshes;
        void loadModel(std::string path);
        
    private:
        
        void processNode(aiNode *node, const aiScene *scene);
        Mesh* processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};

#endif