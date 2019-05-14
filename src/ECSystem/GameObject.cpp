#include "GameObject.h"

#include "EntityComponentManager.h"


int GameObject::gameObjectId_ = 0;

GameObject::GameObject(const std::string& name, const std::string& parentTag, const std::string& tag, bool active) :
	node_(nullptr),
	parentTag_(parentTag),
	name_(name),
	tag_(tag),
	active_(active),
	disposable_(false),
	destroyable_(true)
{
	uniqueId_ = name_ + std::to_string(gameObjectId_);
	gameObjectId_++;
}

GameObject::~GameObject()
{
}

void GameObject::setActive(bool active)
{
	active_ = active;
	//if (!active_) node_->setVisible(false);
}

GameObject * GameObject::clone()
{
	GameObject* clonedObject = new GameObject(this->name_, this->parentTag_, this->tag_, this->active_);

	if (clonedObject->parentTag_ == "") clonedObject->node_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + clonedObject->uniqueId_);
	else clonedObject->node_ = EntityComponentManager::Instance()->findGameObjectsWithTag(clonedObject->parentTag_).front()->transform()->createChildSceneNode("n" + clonedObject->uniqueId_);
	EntityComponentManager::Instance()->addEntity(clonedObject);

	return clonedObject;
}
