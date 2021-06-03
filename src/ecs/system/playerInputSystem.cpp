#include "playerInputSystem.h"
#include "ecs/comp/playerInputComp.h"
#include "ecs/comp/moveComp.h"
#include <QDebug>

PlayerInputSystem::PlayerInputSystem(entt::registry* registry, MasterRenderer* masterRenderer) : ComponentSystem(registry, masterRenderer)
{
}

void PlayerInputSystem::update(float deltaTime)
{
	auto view = m_ecs->view<MoveComp, PlayerInputComp>();
	for (auto entity : view) {
		auto [move, player] = view.get<MoveComp, PlayerInputComp>(entity);
		if (InputManager::getInstance().isPressed(TREEUP)) {
			move.velocity = player.velocity * deltaTime;
			move.velRelativeToSelf = true;
		}
		else if (InputManager::getInstance().isPressed(TREEDOWN)) {
			move.angularVelocity = player.velocity * deltaTime;
			move.angularVelRelativeToSelf = true;
		}
		
	}

}