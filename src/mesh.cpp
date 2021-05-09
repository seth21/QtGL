#include "mesh.h"
#include <iostream>

Mesh::Mesh()
{
    
}

Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<ModelTextureDef> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->name = name;
    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
    aabb.findMinMax(vertices);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    this->vertices = vertices;
    this->indices = indices;
    qDebug() << "Uploading mesh to GL";
    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
    aabb.findMinMax(vertices);
}

Mesh::~Mesh()
{
    releaseResources();

}

void Mesh::releaseResources() {
    // here we delete the opengl arrays
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glDeleteVertexArrays(1, &VAO);
    f->glDeleteBuffers(1, &VBO);
    f->glDeleteBuffers(1, &EBO);
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    // Create buffers/arrays
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);
    
    f->glBindVertexArray(VAO);
    // Load data into vertex buffers
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    f->glEnableVertexAttribArray(0);
    // Vertex Normals
    
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    f->glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    f->glEnableVertexAttribArray(2);

    f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
    f->glEnableVertexAttribArray(3);

    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);

}

void Mesh::drawNow(){
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glBindVertexArray(VAO);
    f->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
}