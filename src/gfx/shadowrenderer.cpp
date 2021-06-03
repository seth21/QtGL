#include "shadowrenderer.h"

ShadowRenderer::ShadowRenderer()
{
	f = QOpenGLContext::currentContext()->extraFunctions();
	dirLightShader = ResourceManager::getInstance().load<ShaderProgram>("gdirlight");
	pointLightShader = ResourceManager::getInstance().load<ShaderProgram>("gpointlight");
	dirLightDepthShader = ResourceManager::getInstance().load<ShaderProgram>("dirlightdepth");
}

void ShadowRenderer::addDirectionalLight(DirectionalLight* light)
{
	dirShadowQueues.push_back(DirShadowQueue(light));
}

void ShadowRenderer::addPointLight(PointLight* light)
{
	pointShadowQueues.push_back(PointShadowQueue(light));
}

void ShadowRenderer::addShadowCaster(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material* material, BoundingSphere *boundingSphere)
{
	//add to shadow caster queue if the light can "see" it
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight* light = dirShadowQueues[i].m_light;
		//if (light->getShadowBox()->getFrustum()->boundsInFrustum(*transformedAABB)) {
			dirShadowQueues[i].queue.createCommand(vaoID, baseVertex, vertexCount, transformMatrix, material, 1);
		//}
	}

	for (int i = 0; i < pointShadowQueues.size(); i++) {
		PointLight* light = pointShadowQueues[i].m_light;
		if (light->bSphere.intersect(*boundingSphere)) {
			pointShadowQueues[i].queue.createCommand(vaoID, baseVertex, vertexCount, transformMatrix, material, 1);
		}
	}
}

void ShadowRenderer::updateDirectionalLights(Camera* cam)
{
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight* light = dirShadowQueues[i].m_light;
		light->updateMatrices(cam);
	}
}

void ShadowRenderer::updatePointLights()
{
}

void ShadowRenderer::clearQueues()
{
	dirShadowQueues.clear();
	pointShadowQueues.clear();
}

void ShadowRenderer::doDirectionalLightDepthPass()
{
	//Prepare state
	f->glEnable(GL_DEPTH_TEST);
	f->glDepthMask(GL_TRUE);
	f->glDepthFunc(GL_LEQUAL);
	f->glEnable(GL_CULL_FACE);
	f->glCullFace(GL_FRONT);
	dirLightDepthShader->start();
	
	//Render directional shadowmaps
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight *dirLight = dirShadowQueues[i].m_light;
		dirLight->getShadowMap()->bind();
		f->glViewport(0, 0, dirLight->getShadowMapSize(), dirLight->getShadowMapSize());
		f->glClear(GL_DEPTH_BUFFER_BIT);
		dirLightDepthShader->loadMatrix4f("projMat", dirLight->getProjectionMatrix());
		dirLightDepthShader->loadMatrix4f("lightViewMat", dirLight->getLightViewMatrix());
		auto &commands = dirShadowQueues[i].queue.getCommands();
		for (auto &command : commands) {
			dirLightDepthShader->loadMatrix4f("modelMat", command.transformMatrix);
			f->glBindVertexArray(command.vaoID);
			f->glDrawElements(GL_TRIANGLES, command.vertexCount, GL_UNSIGNED_INT, (GLvoid*)command.baseVertex);
		}
		
	}
	
}

void ShadowRenderer::doPointLightsDepthPass()
{
}
