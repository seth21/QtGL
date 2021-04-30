#include "shadowrenderer.h"

ShadowRenderer::ShadowRenderer()
{
	initializeOpenGLFunctions();
	dirLightShader = ResourceManager::getInstance().load<ShaderProgram>("gdirlight");
	pointLightShader = ResourceManager::getInstance().load<ShaderProgram>("gpointlight");
	dirLightDepthShader = ResourceManager::getInstance().load<ShaderProgram>("dirlightdepth");
}

void ShadowRenderer::addDirectionalLight(DirectionalLight* light)
{
	dirShadowQueues.push_back(ShadowQueue(light));
}

void ShadowRenderer::addShadowCaster(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material* material, BoundingBox *transformedAABB)
{
	//add to shadow caster queue if the light can "see" it
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight* light = dirShadowQueues[i].m_light;
		//if (light->getShadowBox()->getFrustum()->boundsInFrustum(*transformedAABB)) {
			dirShadowQueues[i].queue.createCommand(vaoID, baseVertex, vertexCount, transformMatrix, material, 1);
		//}
	}
}

void ShadowRenderer::updateDirectionalLights(Camera* cam)
{
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight* light = dirShadowQueues[i].m_light;
		light->updateMatrices(cam);
	}
}

void ShadowRenderer::clearQueues()
{
	dirShadowQueues.clear();
}

void ShadowRenderer::doDirectionalLightDepthPass()
{
	//Prepare state
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	dirLightDepthShader->start();
	
	//Render directional shadowmaps
	for (int i = 0; i < dirShadowQueues.size(); i++) {
		DirectionalLight *dirLight = dirShadowQueues[i].m_light;
		dirLight->getShadowMap()->bind();
		glViewport(0, 0, dirLight->getShadowMapSize(), dirLight->getShadowMapSize());
		glClear(GL_DEPTH_BUFFER_BIT);
		dirLightDepthShader->loadMatrix4f("projMat", dirLight->getProjectionMatrix());
		dirLightDepthShader->loadMatrix4f("lightViewMat", dirLight->getLightViewMatrix());
		auto &commands = dirShadowQueues[i].queue.getCommands();
		for (auto &command : commands) {
			dirLightDepthShader->loadMatrix4f("modelMat", command.transformMatrix);
			glBindVertexArray(command.vaoID);
			glDrawElements(GL_TRIANGLES, command.vertexCount, GL_UNSIGNED_INT, (GLvoid*)command.baseVertex);
		}
		
	}
	
}