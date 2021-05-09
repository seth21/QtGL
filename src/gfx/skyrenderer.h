#ifndef SKYRENDERER_H
#define SKYRENDERER_H
#include <memory>
#include "shaderprogram.h"
#include "texture.h"
#include "camera.h"
#include <QOpenGLExtraFunctions>
#include "vao.h"
#include "resourcemanager.h"
#include "framebuffer.h"

class SkyRenderer {
public:
	SkyRenderer();
	~SkyRenderer();
	void render(Camera* cam, FrameBuffer* target);
	void setSkyTexture(std::shared_ptr<Texture> skyTex);
private:
	std::shared_ptr<ShaderProgram> skyShader;
	std::shared_ptr<Texture> skyTexture;

	void generateCubeMesh();
	std::unique_ptr<VAO> cubeVAO;
	const static float cubeVertices[];
	QOpenGLExtraFunctions* f;
};

#endif