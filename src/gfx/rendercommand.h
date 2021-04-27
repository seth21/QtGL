#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H
#include <glm/mat4x4.hpp>
class Material;

struct RenderCommand {
	unsigned int vaoID;
	unsigned baseVertex;
	unsigned int vertexCount;
	float distanceSq;
	Material* material;
	glm::mat4x4 transformMatrix;
};

#endif