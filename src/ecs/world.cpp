#include "world.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/hoverComp.h"
#include "ecs/comp/meshrendererComp.h"
#include "ecs/comp/directionallightComp.h"
#include "ecs/comp/pointlightComp.h"
#include "ecs/comp/cameraComp.h"
#include "ecs/system/lightSystem.h"
#include "ecs/system/renderSystem.h"
#include "ecs/system/transformSystem.h"
#include "ecs/system/transformUndirtySystem.h"
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
	auto transformSystem = std::make_unique<TransformSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(transformSystem));
	auto lightSystem = std::make_unique<LightSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(lightSystem));
	auto cameraSystem = std::make_unique<CameraSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(cameraSystem));
	auto renderSystem = std::make_unique<RenderSystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(renderSystem));
	auto transformUndirtySystem = std::make_unique<TransformUndirtySystem>(&m_Registry, masterRenderer.get());
	systems.push_back(std::move(transformUndirtySystem));
	//ENTITIES

	sponza = loadModelToEntity("models/medieval-scenery/medievalApplied.fbx");
	auto& sceneTrans = m_Registry.get<TransformComp>(sponza);
	sceneTrans.setScale(glm::vec3(0.05));

	dirLight = m_Registry.create();
	m_Registry.emplace<DirectionalLightComp>(dirLight);

	player = m_Registry.create();
	m_Registry.emplace<CameraComp>(player);

	pointLight = m_Registry.create();
	m_Registry.emplace<PointLightComp>(pointLight);
	auto& transPointLight = m_Registry.emplace<TransformComp>(pointLight);
	transPointLight.setPosition(glm::vec3(0, 4, 0));

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
	trans.setPosition(node->position);
	trans.setScale(node->scale);
	trans.setOrientation(node->orientation);
	trans.parent = parent;
	trans.hierarchyDepth = depth;
	if (parent != entt::null) m_Registry.get<TransformComp>(parent).getChildren().push_back(e);
	if (node->meshIndices.size() > 0) m_Registry.emplace<MeshRendererComp>(e, model, node->meshIndices, node->name);
	qDebug() << "Name:" << node->name.c_str() << " Mesh indices:" << node->meshIndices.size() << " Children:" << node->children.size();
	//Add children
	entt::entity lastSibling = entt::null;
	for (int i = 0; i < node->children.size(); i++) {
		entt::entity child = createMeshRendererFromNode(node->children[i].get(), e, model, depth + 1);
	}
		/*if (lastSibling == entt::null) {
			//This is the first child of the parent
			trans.first_child = child;
		}
		else {
			//Get the previous sibling
			auto& transPrevSibling = m_Registry.get<TransformComp>(lastSibling);
			//And assign the current child as the next sibling
			transPrevSibling.next_sibling = child;
			//Get the current sibling
			auto& transCurrSibling = m_Registry.get<TransformComp>(child);
			//And assign the last child as the previous sibling
			transCurrSibling.prev_sibling = lastSibling;
		}

		lastSibling = child;
	}*/
	
	return e;
}

