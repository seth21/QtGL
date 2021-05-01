#include "world.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/hoverComp.h"
#include "ecs/comp/meshrendererComp.h"
#include "ecs/comp/directionallightComp.h"
#include "ecs/comp/cameraComp.h"
#include "ecs/system/lightSystem.h"
#include "ecs/system/renderSystem.h"
#include <QDebug>
#include "fxaa.h"
#include "colorcorrection.h"

World::World(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	masterRenderer = std::make_unique<MasterRenderer>(x, y, width, height);
	
	setupScene();
}

void World::setupScene()
{
	//SYSTEMS
	auto lightSystem = std::make_unique<LightSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(lightSystem));
	auto cameraSystem = std::make_unique<CameraSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(cameraSystem));
	auto renderSystem = std::make_unique<RenderSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(renderSystem));
	//ENTITIES
	sponza = m_Registry.create();
	auto &trans = m_Registry.emplace<TransformComp>(sponza);
	trans.scale = glm::vec3(0.05f, 0.05f, 0.05f);
	auto &meshComp = m_Registry.emplace<MeshRendererComp>(sponza, ResourceManager::getInstance().load<Model>("models/sponza.obj"));
	for (auto mat : meshComp.m_model->materials) {
		mat->state.set(MaterialFlag::CASTSHADOW);
	}
	
	dirLight = m_Registry.create();
	m_Registry.emplace<DirectionalLightComp>(dirLight);

	player = m_Registry.create();
	m_Registry.emplace<CameraComp>(player);

	pointLight = m_Registry.create();
	m_Registry.emplace<PointLightComp>(pointLight);

	ResourceConfig cubeConfig;
	cubeConfig.flags.push_back("cube");
	auto cubeTex = ResourceManager::getInstance().load<Texture>("textures/skybox/day/*right.jpg*left.jpg*top.jpg*bottom.jpg*front.jpg*back.jpg", cubeConfig);
	masterRenderer->getSkyRenderer()->setSkyTexture(cubeTex);

	FXAA* fxaa = masterRenderer->getPostEffectRenderer()->addEffect<FXAA>();
	ColorCorrection *correction = masterRenderer->getPostEffectRenderer()->addEffect<ColorCorrection>();
	correction->exposure = 0.5f;
	masterRenderer->getDeferredRenderer()->setExposure(0.5f);
}

void World::setViewport(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	masterRenderer->setViewport(x, y, width, height);
}

entt::registry& World::getECS()
{
	return m_Registry;
}

std::vector<std::unique_ptr<ComponentSystem>>& World::getSystems()
{
	return systems;
}

entt::entity World::createEntity()
{
	return m_Registry.create();
}

void World::render(float deltaTime)
{
	
	masterRenderer->render(deltaTime);
}

void World::update(float deltaTime)
{
	for (int i = 0; i < systems.size(); i++) {
		systems[i]->update(deltaTime);
	}
	
	//ecs-update lights if necessary
	//masterRenderer->addLight(light);

	//else
	
}
