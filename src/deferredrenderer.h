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
#include "directionallight.h"
#include "vao.h"
#include "vertexattrib.h"
#include "ssao.h"

class DeferredRenderer : protected QOpenGLExtraFunctions {
public:
	DeferredRenderer(int width, int height);
	~DeferredRenderer();
	void render(Camera *cam, Entity *entity);
	void setViewport(int x, int y, int width, int height);
	DebugRenderer* getDebugRenderer();
private:
	//void setupScreenQuad();
	std::unique_ptr<FrameBuffer> gBuffer;
	std::shared_ptr<ShaderProgram> gBufferShader;
	std::shared_ptr<ShaderProgram> dirLightShader;
	std::shared_ptr<ShaderProgram> dirLightDepthShader;
	std::shared_ptr<ShaderProgram> pointLightShader;
	std::shared_ptr<ShaderProgram> combineShader;
	std::unique_ptr<DebugRenderer> debugRenderer;
	std::unique_ptr<Entity> pointLight;
	std::unique_ptr<DirectionalLight> dirLight;
	std::unique_ptr<VAO> screenVAO;
	std::unique_ptr<SSAO> ssao;
	//unsigned int screenVAO, screenVBO;
	void doGeometryPass(Camera* cam, Entity* entity);
	void doDirectionalLightPass(Camera* cam, Entity* entity);
	void doDirectionalLightDepthPass(Camera* cam, Entity* entity);
	void doPointLightPass(Camera* cam, Entity* entity);
	void doCombinePass(Camera* cam, Entity* entity);
	void doDebugPass(Camera* cam, Entity* entity);
	void doSSAO(Camera* cam);
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
	float exposure = 0.5;
	
};
#endif