#include "MeshRenderer.h"
#include "entitycomponentmanager.h"
#include "Transform.h"

std::string MeshRenderer::name_ = "MeshRenderer";

MeshRenderer::MeshRenderer() :
	entity_(nullptr)
{
}

MeshRenderer::MeshRenderer(GameObject* container, const std::string& meshName, bool enabled) : 
	Component(container, enabled)
{
	entity_ = RenderManager::Instance()->getSceneManager()->createEntity(meshName);
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(entity_);
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::init(const std::map<std::string, ValueType>& params)
{
	entity_ = RenderManager::Instance()->getSceneManager()->createEntity(params.at("mesh").s);
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(entity_);
	setEnabled(params.at("enabled").b);
}
