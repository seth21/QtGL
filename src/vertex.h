#ifndef VERTEX_H
#define VERTEX_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
    // Tangent
    glm::vec3 Tangent;
};

#endif