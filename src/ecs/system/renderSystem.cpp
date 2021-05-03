#include "renderSystem.h"
#include "ecs/comp/meshrendererComp.h"
#include "ecs/comp/transformComp.h"
#include <QDebug>
RenderSystem::RenderSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void RenderSystem::update(float deltaTime)
{
	auto group = m_ecs->group<TransformComp>(entt::get<MeshRendererComp>);
	for (auto entity : group) {
		auto [transform, meshrenderer] = group.get<TransformComp, MeshRendererComp>(entity);
		if (transform.dirty) {
			transform.transformMatrix = getTRSMatrix(transform.position, transform.orientation, transform.scale);
		}
        m_renderer->enqueueComp(&transform, &meshrenderer);
	}
	
}

glm::mat4 RenderSystem::getTRSMatrix(glm::vec3 &position, glm::quat &rotation, glm::vec3 &scale)
{
    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
	//qDebug() << scale.x << "," << scale.y << "," << scale.z;
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 trans = glm::translate(glm::mat4(1.0), position);
    return trans * rot * scl;

}