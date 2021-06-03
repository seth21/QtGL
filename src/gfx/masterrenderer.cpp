#include "masterrenderer.h"

MasterRenderer::MasterRenderer(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	shadowRenderer = std::make_unique<ShadowRenderer>();
	deferredRenderer = std::make_unique<DefRenderer>(x, y, width, height);
	ssao = std::make_unique<SSAO>(width, height);
	postRenderer = std::make_unique<PostEffectRenderer>(x, y, width, height);
	skyRenderer = std::make_unique<SkyRenderer>();
}

/*Ensure the camera and lights are updated before adding to queue
*/
void MasterRenderer::enqueueComp(TransformComp* transformComp, MeshRendererComp* rendererComp)
{
	//check type
	//add to appropriate queue
	auto &meshIndices = rendererComp->meshIndices;
	Model* model = rendererComp->m_model.get();
	for (unsigned int meshIndex : meshIndices) {

		Mesh* mesh = model->meshes[meshIndex];
		Material* mat = model->materials[mesh->materialIndex];
		//a
		BoundingBox& transformedAABB = transformComp->getBoundingBox();
		transformedAABB.setAABB(mesh->aabb);
		transformedAABB.mul(transformComp->transformMatrix);
		transformedAABB.calculateCorners();
		transformComp->getBoundingSphere().calculateSphereApproximate(transformedAABB);
		//Can the camera see the object?
		if (camera->frustum->boundsInFrustum(transformedAABB)) {
			if (mat->blending == RenderBlend::STANDARD || mat->blending == RenderBlend::ADDITIVE) {
				//add to forward blend queue
				forwardBlendQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			}
			//else if (mat->state.test(MaterialFlag::CUSTOM)) {
				//add to forward opaque queue
			//	forwardOpaqueQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			//}
			else {
				//add to deferred queue
				deferredRenderer->getQueue().createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			}
		}

		if (mat->state[MaterialFlag::CASTSHADOW]) {
			shadowRenderer->addShadowCaster(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, &transformComp->getBoundingSphere());
		}
	}
	
}

void MasterRenderer::update(Camera* cam)
{
	camera = cam;
	shadowRenderer->updateDirectionalLights(cam);
	shadowRenderer->updatePointLights();
}

void MasterRenderer::clearCamera()
{
	camera = nullptr;
}

void MasterRenderer::render(float deltaTime)
{
	//Shadow Casters
	shadowRenderer->doDirectionalLightDepthPass();
	deferredRenderer->doGeometryPass(camera);
	ssao->calculateSSAO(deferredRenderer->getGBuffer(), deferredRenderer->getScreenVAO(), camera->getProjMatrix(), camera->getViewMatrix());
	deferredRenderer->doDeferredLighting(camera, ssao.get());
	deferredRenderer->doCombinePass();
	skyRenderer->render(camera, deferredRenderer->getGBuffer());
	if (postRenderer->getActiveEffectsCount() != 0) {
		postRenderer->render(camera, deferredRenderer->getGBuffer(), deferredRenderer->getScreenVAO());
	}
	else {
		//blit to screen
		//skyRenderer->render(camera, deferredRenderer->getGBuffer(), 2);
	}
	//Clear Queues
	forwardBlendQueue.clear();
	forwardOpaqueQueue.clear();
	deferredRenderer->clearQueue();
	shadowRenderer->clearQueues();
}

void MasterRenderer::addLight(DirectionalLight* dirLight)
{
	shadowRenderer->addDirectionalLight(dirLight);
	deferredRenderer->addDirectionalLight(dirLight);
	
}

void MasterRenderer::addLight(PointLight* pointLight)
{
	shadowRenderer->addPointLight(pointLight);
	deferredRenderer->addPointLight(pointLight);
}

void MasterRenderer::setViewport(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	deferredRenderer->setViewport(x, y, width, height);
	postRenderer->setViewport(x, y, width, height);
	ssao->screenSizeChanged(width, height);
}

PostEffectRenderer* MasterRenderer::getPostEffectRenderer()
{
	return postRenderer.get();
}

DefRenderer* MasterRenderer::getDeferredRenderer()
{
	return deferredRenderer.get();
}

DebugRenderer* MasterRenderer::getDebugRenderer()
{
	return debugRenderer.get();
}

SkyRenderer* MasterRenderer::getSkyRenderer()
{
	return skyRenderer.get();
}
