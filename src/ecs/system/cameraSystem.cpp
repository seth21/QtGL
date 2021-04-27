#include "cameraSystem.h"

CameraSystem::CameraSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
	InputManager::getInstance().registerHandler(this);
}

void CameraSystem::update(float deltaTime)
{
	auto view = m_ecs->view<CameraComp>();
	for (auto entity : view) {
		auto &camComp = view.get<CameraComp>(entity);
		if (m_dx != 0 || m_dy != 0) {
			camComp.camera.handleMouseDelta(m_dx, m_dy);
			m_dx = 0;
			m_dy = 0;
		}
		camComp.camera.update(deltaTime);
		m_renderer->update(&(camComp.camera));
	}

}

void CameraSystem::handleAction(ACTION action, bool pressed)
{
}

void CameraSystem::handleMouseDelta(int dx, int dy)
{
	m_dx = dx;
	m_dy = dy;
}
