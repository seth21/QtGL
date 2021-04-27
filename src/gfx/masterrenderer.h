#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H
#include "commandqueue.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/meshrendererComp.h"
#include "camera.h"
#include "directionallight.h"
#include "shadowrenderer.h"
#include "defrenderer.h"
#include "ssao.h"
#include "posteffectrenderer.h"

class MasterRenderer {
public:
	MasterRenderer(int x, int y, int width, int height);
	void enqueueComp(TransformComp* transformComp, MeshRendererComp* rendererComp);
	void update(Camera* cam);
	void clearCamera();
	void render(float deltaTime);
	void addLight(DirectionalLight* dirLight);
	void addLight(PointLight* pointLight);
	void setViewport(int x, int y, int width, int height);
	PostEffectRenderer* getPostEffectRenderer();
	DefRenderer* getDeferredRenderer();
private:
	
	CommandQueue forwardOpaqueQueue;
	CommandQueue forwardBlendQueue;
	Camera* camera;
	
	//SUB-RENDERERS
	std::unique_ptr<LightRenderer> shadowRenderer;
	std::unique_ptr<DefRenderer> deferredRenderer;
	std::unique_ptr<SSAO> ssao;
	std::unique_ptr<PostEffectRenderer> postRenderer;

	int m_x, m_y, m_width, m_height;
};

#endif