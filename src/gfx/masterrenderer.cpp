#include "masterrenderer.h"

MasterRenderer::MasterRenderer(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	shadowRenderer = std::make_unique<LightRenderer>();
	deferredRenderer = std::make_unique<DefRenderer>(x, y, width, height);
	ssao = std::make_unique<SSAO>(width, height);
	postRenderer = std::make_unique<PostEffectRenderer>(x, y, width, height);
}

/*Ensure the camera and lights are updated before adding to queue
*/
void MasterRenderer::enqueueComp(TransformComp* transformComp, MeshRendererComp* rendererComp)
{
	//check type
	//add to appropriate queue
	int meshIndex = rendererComp->m_meshIndex;
	Model* model = rendererComp->m_model.get();
	if (meshIndex >= 0) {
		Material* mat = model->materials[meshIndex];
		Mesh* mesh = model->meshes[meshIndex];
		//a
		BoundingBox& transformedAABB = rendererComp->transformedAABB;
		transformedAABB.setAABB(mesh->aabb);
		transformedAABB.mul(transformComp->transformMatrix);

		//Can the camera see the object?
		if (camera->frustum->boundsInFrustum(transformedAABB)) {
			if (mat->state.test(MaterialFlag::BLENDING)) {
				//add to forward blend queue
				forwardBlendQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			}
			else if (mat->state.test(MaterialFlag::CUSTOM)) {
				//add to forward opaque queue
				forwardOpaqueQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			}
			else {
				//add to deferred queue
				deferredRenderer->getQueue().createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
			}
		}

		if (mat->state.test(MaterialFlag::CASTSHADOW)) {
			shadowRenderer->addShadowCaster(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, &transformedAABB);
		}
	}
	else {
		int meshesSize = model->meshes.size();
		for (int i = 0; i < meshesSize; i++) {
			Mesh* mesh = model->meshes[i];
			Material* mat = model->materials[mesh->materialIndex];

			BoundingBox& transformedAABB = rendererComp->transformedAABB;
			transformedAABB.setAABB(mesh->aabb);
			transformedAABB.mul(transformComp->transformMatrix);

			//Can the camera see the object?
			if (camera->frustum->boundsInFrustum(transformedAABB)) {
				if (mat->state.test(MaterialFlag::BLENDING)) {
					//add to forward blend queue
					forwardBlendQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
				}
				else if (mat->state.test(MaterialFlag::CUSTOM)) {
					//add to forward opaque queue
					forwardOpaqueQueue.createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
				}
				else {
					//add to deferred queue
					deferredRenderer->getQueue().createCommand(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, 0);
				}
			}

			if (mat->state.test(MaterialFlag::CASTSHADOW)) {
				shadowRenderer->addShadowCaster(mesh->VAO, 0, mesh->indices.size(), transformComp->transformMatrix, mat, &transformedAABB);
			}
		}
	}
	
}

void MasterRenderer::update(Camera* cam)
{
	camera = cam;
	shadowRenderer->updateDirectionalLights(cam);
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
	deferredRenderer->doDirectionalLightPass(camera, ssao.get());
	deferredRenderer->doPointLightPass(camera);
	deferredRenderer->doCombinePass(postRenderer->getActiveEffectsCount() != 0 ? postRenderer->getDefaultFBO() : nullptr);
	if (postRenderer->getActiveEffectsCount() != 0) postRenderer->render(camera, deferredRenderer->getGBuffer(), deferredRenderer->getScreenVAO());
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
