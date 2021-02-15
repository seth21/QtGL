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

class Mesh : protected QOpenGLExtraFunctions
{
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<ModelTextureDef> textures;
    std::string name;
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<ModelTextureDef> textures);
    ~Mesh();
    void drawNow();
    void setupMesh();
    /*  Render data  */
    GLuint VAO, VBO, EBO;
private:

    void releaseResources();
    

};

#endif // MESH_H