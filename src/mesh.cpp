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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    initializeOpenGLFunctions();
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Vertex Normals
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Mesh::drawNow(){
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
}