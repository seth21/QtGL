#include "world.h"
#include "transformComp.h"
#include "hoverComp.h"
#include <QDebug>

World::World()
{
	e = m_Registry.create();

	m_Registry.emplace<TransformComp>(e);
	m_Registry.emplace<HoverComp>(e);
}

void World::update(float deltaTime)
{
	auto group = m_Registry.group<TransformComp>(entt::get<HoverComp>);
	for (auto entity : group) {
		auto&[transform, hover] = group.get<TransformComp, HoverComp>(entity);
		hover.current += deltaTime;
		transform.position.y = sin(hover.current * hover.speed) * hover.range;
		qDebug() << transform.position.y;
	}
}
