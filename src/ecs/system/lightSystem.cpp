#include "lightSystem.h"
#include "ecs/comp/directionallightComp.h"
#include <ecs/comp/pointlightComp.h>
#include "ecs/comp/transformComp.h"

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

	/*auto viewp = m_ecs->view<PointLightComp>();
	for (auto entity : viewp) {
		PointLightComp& pLightComp = viewp.get<PointLightComp>(entity);
		m_renderer->addLight(pLightComp.pointLight.get());
	}*/

	auto plview = m_ecs->view<TransformComp, PointLightComp>();
	for (auto entity : plview) {
		auto [transform, pointlight] = plview.get<TransformComp, PointLightComp>(entity);

		//m_renderer->addLight(pointlight.pointLight.get());
		//if (transform.isDirty()) {
			//if light moved set as dirtyLight
		//	pointlight.setDirty(true);
			//check all entities if they intersect->recalculate affectedEntities from the start
			//pointlight.affectedEntities.clear();

		//}
		//else {

		//}
	}

	
}
/*
for every light -> if light moved set as dirtyLight
check all entities if they intersect->recalculate affectedEntities from the start
else if light didnt move
check all transform dirty entities ->
if they are inside radius
dirty the light
if entity didnt exist add to affected entities
if not inside radius
check if it was inside affected entities - if yes remove from listand dirty the light
if light dirty AND visible from camera re - render shadowmap and undirty it
*/