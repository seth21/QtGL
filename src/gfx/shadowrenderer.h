#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H
#include "resourcemanager.h"
#include <QOpenGLExtraFunctions>
#include "shaderprogram.h"
#include "directionallight.h"
#include "lights/pointlight.h"
#include "commandqueue.h"
#include <ecs/comp/transformComp.h>
#include <ecs/comp/meshrendererComp.h>
struct DirShadowQueue {
	DirectionalLight* m_light;
	CommandQueue queue;
	DirShadowQueue(DirectionalLight* light) {
		m_light = light;
	}
};

struct PointShadowQueue {
	PointLight* m_light;
	CommandQueue queue;
	PointShadowQueue(PointLight* light) {
		m_light = light;
	}
};

class ShadowRenderer {
public:

	ShadowRenderer();
	void addDirectionalLight(DirectionalLight* light);
	void addPointLight(PointLight* light);
	void addShadowCaster(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material* material, BoundingSphere* boundingSphere);
	void updateDirectionalLights(Camera* cam);
	void updatePointLights();
	void doDirectionalLightDepthPass();
	void doPointLightsDepthPass();
	void clearQueues();
	
private:
	QOpenGLExtraFunctions* f;
	//SHADERS
	std::shared_ptr<ShaderProgram> dirLightShader;
	std::shared_ptr<ShaderProgram> dirLightDepthShader;
	std::shared_ptr<ShaderProgram> pointLightShader;
	//QUEUES
	std::vector<DirShadowQueue> dirShadowQueues;
	std::vector<PointShadowQueue> pointShadowQueues;
};

#endif