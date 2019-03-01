#include "MeshRenderer.h"
#include "entitycomponentmanager.h"
#include "Transform.h"

std::string MeshRenderer::name_ = "MeshRenderer";

MeshRenderer::MeshRenderer(GameObject* container, const std::string& meshName, bool enabled) : Component(container, enabled)
{
	entity_ = Application::Instance()->getSceneManager()->createEntity(meshName);
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(entity_);
}

MeshRenderer::~MeshRenderer()
{

}