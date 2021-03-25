#ifndef MESH_H
#define MESH_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include "vertex.h"
#include "modeltexturedef.h"
#include "material.h"
#include "boundingbox.h"

class Mesh : protected QOpenGLExtraFunctions
{
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<ModelTextureDef> textures;
    std::string name;
    BoundingBox aabb;
    BoundingBox transformedAabb;
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<ModelTextureDef> textures);
    ~Mesh();
    void drawNow();
    void setupMesh();
    /*  Render data  */
    GLuint VAO, VBO, EBO;
    unsigned int materialIndex = 0;
    unsigned int baseVertex = 0;
    unsigned int vertexCount = 0;
private:

    void releaseResources();
    

};

#endif // MESH_H