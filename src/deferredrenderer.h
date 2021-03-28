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
#include "debugrenderer.h"
#include <QDebug>

class DeferredRenderer : protected QOpenGLExtraFunctions {
public:
	DeferredRenderer(int width, int height);
	~DeferredRenderer();
	void render(Camera *cam, Entity *entity);
	void setViewport(int x, int y, int width, int height);
	DebugRenderer* getDebugRenderer();
private:
	void setupScreenQuad();
	std::unique_ptr<FrameBuffer> gBuffer;
	std::shared_ptr<ShaderProgram> gBufferShader;
	std::shared_ptr<ShaderProgram> dirLightShader;
	std::shared_ptr<ShaderProgram> pointLightShader;
	std::shared_ptr<ShaderProgram> combineShader;
	std::unique_ptr<DebugRenderer> debugRenderer;
	std::unique_ptr<Entity> pointLight;
	unsigned int screenVAO, screenVBO;
	void doGeometryPass(Camera* cam, Entity* entity);
	void doDirectionalLightPass(Camera* cam, Entity* entity);
	void doPointLightPass(Camera* cam, Entity* entity);
	void doCombinePass(Camera* cam, Entity* entity);
	void doDebugPass(Camera* cam, Entity* entity);
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
	float exposure = 0.5;
	
};
#endif