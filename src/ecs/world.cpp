#include "world.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/hoverComp.h"
#include "ecs/comp/meshrendererComp.h"
#include "ecs/comp/directionallightComp.h"
#include "ecs/comp/pointlightComp.h"
#include "ecs/comp/playerInputComp.h"
#include "ecs/comp/cameraComp.h"
#include "ecs/comp/moveComp.h"
#include "ecs/comp/hierarchyComp.h"
#include "ecs/system/lightSystem.h"
#include "ecs/system/renderSystem.h"
#include "ecs/system/transformSystem.h"
#include "ecs/system/hierarchySystem.h"
#include "ecs/system/movementSystem.h"
#include "ecs/system/playerInputSystem.h"
#include <QDebug>
#include "fxaa.h"
#include "colorcorrection.h"
#include <gfx/ssr.h>

World::World(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	masterRenderer = std::make_unique<MasterRenderer>(x, y, width, height);
	
	setupScene();
}

void World::update(float deltaTime)
{
	for (int i = 0; i < systems.size(); i++) {
		systems[i]->update(deltaTime);
	}
	
}

void World::render(float deltaTime)
{
	masterRenderer->render(deltaTime);
}

void World::setupScene()
{


	//SYSTEMS
	auto playerInputSystem = std::make_unique<PlayerInputSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(playerInputSystem));
	auto hierarchySystem = std::make_unique<HierarchySystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(hierarchySystem));
	auto movementSystem = std::make_unique<MovementSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(movementSystem));
	auto transformSystem = std::make_unique<TransformSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(transformSystem));
	auto lightSystem = std::make_unique<LightSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(lightSystem));
	auto cameraSystem = std::make_unique<CameraSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(cameraSystem));
	auto renderSystem = std::make_unique<RenderSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(renderSystem));
	
	//ENTITIES

	sponza = loadModelToEntity("models/medieval-scenery/medievalApplied.fbx");
	auto& sceneTrans = m_Registry.get<TransformComp>(sponza);
	m_Registry.patch<TransformComp>(sponza, [](auto& trans) { trans.localScale = glm::vec3(0.05); });
	m_Registry.emplace<MoveComp>((entt::entity)73);
	auto &playerInput = m_Registry.emplace<PlayerInputComp>((entt::entity)73);
	playerInput.velocity = glm::vec3(0, 2, 0);
	
	dirLight = m_Registry.create();
	m_Registry.emplace<DirectionalLightComp>(dirLight);
	
	player = m_Registry.create();
	m_Registry.emplace<CameraComp>(player);
	
	pointLight = m_Registry.create();
	m_Registry.emplace<PointLightComp>(pointLight);
	auto& transPointLight = m_Registry.emplace<TransformComp>(pointLight);
	transPointLight.localPosition = glm::vec3(0, 4, 0);

	ResourceConfig cubeConfig;
	cubeConfig.addFlag("cube");
	auto cubeTex = ResourceManager::getInstance().load<Texture>("textures/skybox/day/*right.jpg*left.jpg*top.jpg*bottom.jpg*front.jpg*back.jpg", cubeConfig);
	masterRenderer->getSkyRenderer()->setSkyTexture(cubeTex);

	//SSR* ssr = masterRenderer->getPostEffectRenderer()->addEffect<SSR>();
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

entt::entity World::loadModelToEntity(std::string modelPath)
{
	auto model = ResourceManager::getInstance().load<Model>(modelPath);
	MeshNode* rootNode = model->meshnode.get();
	//Recursively add the mesh entities
	entt::entity rootEntity = createMeshRendererFromNode(rootNode, entt::null, model, 0);
	//Assign shaders
	for (auto* mat : model->materials) {
		mat->state.set(MaterialFlag::CASTSHADOW);
		ShaderResolver::assignShaderToMaterial(mat);
	}
	return rootEntity;
}

entt::entity World::createMeshRendererFromNode(MeshNode* node, entt::entity parent, std::shared_ptr<Model> model, unsigned char depth)
{
	entt::entity e = m_Registry.create();
	auto& trans = m_Registry.emplace<TransformComp>(e);
	trans.localPosition = node->position;
	trans.localScale = node->scale;
	trans.localOrientation = node->orientation;
	auto& hier = m_Registry.emplace<HierarchyComp>(e, parent);
	
	if (node->meshIndices.size() > 0) m_Registry.emplace<MeshRendererComp>(e, model, node->meshIndices, node->name);
	//qDebug() << "ID:" << (int)e << "Name:" << node->name.c_str() << " Mesh indices:" << node->meshIndices.size() << " Children:" << node->children.size();
	//Add children
	entt::entity lastSibling = entt::null;
	for (int i = 0; i < node->children.size(); i++) {
		entt::entity child = createMeshRendererFromNode(node->children[i].get(), e, model, depth + 1);
	}
	
	return e;
}

