#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

class Material;

struct RenderCommand {
	unsigned int vaoID;
	unsigned baseVertex, vertexCount;
	//std
	float distance;
	Material* material;
};

#endif