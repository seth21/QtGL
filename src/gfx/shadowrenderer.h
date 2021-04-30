#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H
#include "resourcemanager.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "shaderprogram.h"
#include "directionallight.h"
#include "commandqueue.h"
#include <ecs/comp/transformComp.h>
#include <ecs/comp/meshrendererComp.h>
struct ShadowQueue {
	DirectionalLight* m_light;
	CommandQueue queue;
	ShadowQueue(DirectionalLight* light) {
		m_light = light;
	}
};

class ShadowRenderer : protected QOpenGLExtraFunctions {
public:

	ShadowRenderer();
	void addDirectionalLight(DirectionalLight* light);
	void addShadowCaster(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material* material, BoundingBox* transformedAABB);
	void updateDirectionalLights(Camera* cam);
	void doDirectionalLightDepthPass();
	void clearQueues();
	
private:
	
	//SHADERS
	std::shared_ptr<ShaderProgram> dirLightShader;
	std::shared_ptr<ShaderProgram> dirLightDepthShader;
	std::shared_ptr<ShaderProgram> pointLightShader;
	//QUEUES
	std::vector<ShadowQueue> dirShadowQueues;
	std::vector<ShadowQueue> pointShadowQueues;
};

#endif