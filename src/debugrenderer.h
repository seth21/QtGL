#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H
#include <vector>
#include <glm/vec3.hpp>
#include <QOpenGLExtraFunctions>
#include <memory>
#include "camera.h"
#include "shaderprogram.h"
#include "resourcemanager.h"

class DebugRenderer : protected QOpenGLExtraFunctions {
public:
	DebugRenderer();
	~DebugRenderer();
	void glBegin(int mode);
	void glEnd(Camera* cam);
	void glColor3f(float r, float g, float b);
	void glVertex3f(float x, float y, float z);
	void glVertex3f(glm::vec3 vertex);
	void drawLines();
private:
	void uploadFloatsToVBO(unsigned int vbo, std::vector<glm::vec3> &dataList, GLenum usage);
	GLenum drawMode;
	unsigned int vaoID, vboID;
	std::shared_ptr<ShaderProgram> shader;
	int maxVertices = 50000;
	int vertexDataLength = 6;
	std::vector<glm::vec3> dataList;
	glm::vec3 color;

};
#endif