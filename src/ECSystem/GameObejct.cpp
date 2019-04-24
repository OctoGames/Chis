#include "GameObject.h"
#include "EntityComponentManager.h"

int GameObject::game_object_count_ = 0;

GameObject::GameObject(const std::string& name, const std::string& parent, const std::string& tag, bool active) :
	node_(nullptr),
	parent_(parent),
	name_(name),
	tag_(tag),
	active_(active)
{
	unique_id_ = name_ + std::to_string(game_object_count_);
	game_object_count_++;
}

GameObject::~GameObject()
{
}

GameObject * GameObject::clone()
{
	GameObject* clonedObject = new GameObject(this->name_, this->parent_, this->tag_, this->active_);

	if (clonedObject->parent_ == "") clonedObject->node_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + clonedObject->unique_id_);
	else clonedObject->node_ = EntityComponentManager::Instance()->findGameObjectWithName(clonedObject->parent_)->transform()->createChildSceneNode("n" + clonedObject->unique_id_);

	if (clonedObject->tag_ != "") EntityComponentManager::Instance()->addGameObjectWithTag(clonedObject, clonedObject->tag_);
	EntityComponentManager::Instance()->addEntity(clonedObject);

	return clonedObject;
}
