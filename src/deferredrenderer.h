#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <memory>
#include "framebuffer.h"
#include "shaderprogram.h"
#include "camera.h"
#include "entity.h"
#include "resourcemanager.h"
#include <QDebug>

class DeferredRenderer : protected QOpenGLExtraFunctions {
public:
	DeferredRenderer(int width, int height);
	~DeferredRenderer();
	void render(Camera *cam, Entity *entity);
	void setViewport(int x, int y, int width, int height);
private:
	void setupScreenQuad();
	std::unique_ptr<FrameBuffer> gBuffer;
	std::shared_ptr<ShaderProgram> gBufferShader;
	std::shared_ptr<ShaderProgram> dirLightShader;
	unsigned int VAO, VBO;
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
	
};
#endif