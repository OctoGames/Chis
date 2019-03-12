#include "gameobject.h"
#include "entitycomponentmanager.h"

int GameObject::game_object_count_ = 0;

GameObject::GameObject(const std::string& name, const std::string& tag, bool active) :
	name_(name),
	tag_(tag),
	active_(active),
	unique_id_(game_object_count_)
{
	game_object_count_++;
	if (tag_ != "") EntityComponentManager::Instance()->addGameObjectWithTag(this, tag_);
}

GameObject::~GameObject()
{

}