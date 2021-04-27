#include "lightSystem.h"

LightSystem::LightSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void LightSystem::update(float deltaTime)
{
	auto viewD = m_ecs->view<DirectionalLightComp>();
	for (auto entity : viewD) {
		DirectionalLightComp& dirLightComp = viewD.get<DirectionalLightComp>(entity);
		m_renderer->addLight(dirLightComp.dirLight.get());
	}

	auto viewP = m_ecs->view<PointLightComp>();
	for (auto entity : viewP) {
		PointLightComp& pLightComp = viewP.get<PointLightComp>(entity);
		m_renderer->addLight(pLightComp.pointLight.get());
	}
}
