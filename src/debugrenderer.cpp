#include "debugrenderer.h"

DebugRenderer::DebugRenderer() {
	initializeOpenGLFunctions();
	shader = ResourceManager::getInstance().load<ShaderProgram>("debug");

	//VAO setup------
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	//VBO setup------
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	//allocate some memory
	glBufferData(GL_ARRAY_BUFFER, maxVertices * vertexDataLength * sizeof(float), (GLvoid*)0, GL_DYNAMIC_DRAW);
	//where should opengl look for the attributes?
	//vertices
	int posAttribLoc = 0;
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, vertexDataLength * sizeof(float), 0);
	glEnableVertexAttribArray(posAttribLoc);
	//colors
	int colorAttribLoc = 1;
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, vertexDataLength * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colorAttribLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind the vao
	glBindVertexArray(0);
}

DebugRenderer::~DebugRenderer()
{
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	//loader.disposeAsset(shader);
}


void DebugRenderer::glBegin(int mode) {
	drawMode = mode;
}

void DebugRenderer::glEnd(Camera* cam) {
	//prepare
	shader->start();
	shader->loadMatrix4f("pv", cam->getPVmatrix());
	glBindVertexArray(vaoID);

	//renew the vbo's data
	uploadFloatsToVBO(vboID, dataList, GL_DYNAMIC_DRAW);

	//draw
	glDrawArrays(drawMode, 0, dataList.size() / vertexDataLength);
	//Time.registerDrawCall();
	//end
	glBindVertexArray(0);
	shader->stop();

	dataList.clear();
}

void DebugRenderer::glColor3f(float r, float g, float b) {
	color = glm::vec3(r,g,b);
}

void DebugRenderer::glVertex3f(float x, float y, float z) {
	if (dataList.size() > maxVertices) return;
	dataList.push_back(glm::vec3(x, y, z));
	dataList.push_back(color);

}

void DebugRenderer::glVertex3f(glm::vec3 vertex) {
	if (dataList.size() > maxVertices) return;
	dataList.push_back(vertex);
	dataList.push_back(color);

}

void DebugRenderer::uploadFloatsToVBO(unsigned int vbo, std::vector<glm::vec3>& dataList, GLenum usage) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, dataList.size() * sizeof(float), &dataList[0], usage);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataList.size() * sizeof(float) * 3, &dataList[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}