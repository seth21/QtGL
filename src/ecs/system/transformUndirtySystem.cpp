#include "transformUndirtySystem.h"

TransformUndirtySystem::TransformUndirtySystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void TransformUndirtySystem::update(float deltaTime)
{
	auto view = m_ecs->view<TransformComp>();
	for (auto entity : view) {
		TransformComp& transComp = view.get<TransformComp>(entity);
		transComp.setDirty(false);

	}
}