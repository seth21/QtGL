#include "transformSystem.h"

TransformSystem::TransformSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void TransformSystem::update(float deltaTime)
{
	m_ecs->sort<TransformComp>([this](const entt::entity lhs, const entt::entity rhs) {
		const auto& clhs = m_ecs->get<TransformComp>(lhs);
		const auto& crhs = m_ecs->get<TransformComp>(rhs);
		return clhs.hierarchyDepth < crhs.hierarchyDepth;
		});

	auto view = m_ecs->view<TransformComp>();
	for (auto entity : view) {
		TransformComp& transComp = view.get<TransformComp>(entity);
		//Only update if dirty (meaning this transform changed or a parent's)
		if (transComp.isDirty()) {
			glm::mat4 scl = glm::scale(glm::mat4(1.0f), transComp.getScale());
			glm::mat4 rot = glm::mat4_cast(transComp.getOrientation());
			glm::mat4 trans = glm::translate(glm::mat4(1.0), transComp.getPosition());
			if (transComp.parent != entt::null) {
				TransformComp& parentTrans = view.get<TransformComp>(transComp.parent);
				transComp.setWorldMatrix(parentTrans.getWorldMatrix() * trans * rot * scl);
			}
			else transComp.setWorldMatrix(trans * rot * scl);
			
		}
		
		if (first == false) {
			std::string par = transComp.parent == entt::null ? "NULL" : std::to_string((int)transComp.parent);
			qDebug() << "BBBB - Entity:" << std::to_string((int)entity).c_str() << " Parent:" << par.c_str() << " Children:" << transComp.getChildren().size() << "Depth:" << transComp.hierarchyDepth;

		}
		
	}
	first = true;
}

