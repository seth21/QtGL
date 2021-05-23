#include "renderSystem.h"
#include "ecs/comp/meshrendererComp.h"
#include "ecs/comp/transformComp.h"
#include <QDebug>

RenderSystem::RenderSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void RenderSystem::update(float deltaTime)
{
	auto view = m_ecs->view<TransformComp, MeshRendererComp>();
	for (auto entity : view) {
		auto [transform, meshrenderer] = view.get<TransformComp, MeshRendererComp>(entity);
		
		
        m_renderer->enqueueComp(&transform, &meshrenderer);
	}
	
}

