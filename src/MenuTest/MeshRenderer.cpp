#include "MeshRenderer.h"

#include "EntityComponentManager.h"

std::string MeshRenderer::name_ = "MeshRenderer";

MeshRenderer::MeshRenderer() :
	entity_(nullptr),
	meshName_(""),
	materialName_("")
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_mr").b;
	meshName_ = params.at("mesh_name").s;
	materialName_ = params.at("material_name").s;
}

Component * MeshRenderer::clone()
{
	MeshRenderer* clonedComponent = new MeshRenderer();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->meshName_ = this->meshName_;
	clonedComponent->materialName_ = this->materialName_;

	return clonedComponent;
}

void MeshRenderer::init()
{
	entity_ = RenderManager::Instance()->getSceneManager()->createEntity(meshName_);
	entity_->setMaterialName(materialName_);
	gameObject()->transform()->attachObject(entity_);
	setEnabled(enabled_);
}
