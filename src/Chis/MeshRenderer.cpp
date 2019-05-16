#include "MeshRenderer.h"


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

void MeshRenderer::onDisable()
{
	if (entity_) RenderManager::Instance()->getSceneManager()->destroyEntity(entity_);
	entity_ = nullptr;
}

void MeshRenderer::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_mr"); if (it != params.cend()) enabled_ = params.at("enabled_mr").b;
	it = params.find("mesh_name"); if (it != params.cend())	meshName_ = params.at("mesh_name").s;
	it = params.find("material_name"); if (it != params.cend()) materialName_ = params.at("material_name").s;
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
	if (materialName_ != "") entity_->setMaterialName(materialName_);
	gameObject()->transform()->attachObject(entity_);
	setEnabled(enabled_);
}

void MeshRenderer::changeMesh(std::string meshName, std::string materialName)
{
	if (entity_) {
		RenderManager::Instance()->getSceneManager()->destroyEntity(entity_);
		entity_ = nullptr;
	}
	entity_ = RenderManager::Instance()->getSceneManager()->createEntity(meshName);
	if (materialName_ != "") entity_->setMaterialName(materialName_);
	gameObject()->transform()->attachObject(entity_);
	//gameObject()->transform()->scale(30, 30, 30);
	//gameObject()->transform()->rotate(Ogre::Quaternion(Ogre::Radian(180.0f), Ogre::Vector3(0, 1, 0)));
}