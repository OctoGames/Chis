#include "Transform.h"

std::string Transform::name_ = "Transform";

Transform::Transform() :
	node_(nullptr)
{
}

Transform::Transform(GameObject* container, const std::string& parent, bool enabled) : Component(container, enabled)
{
	if (parent == "") node_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + gameObject()->getName());
	else node_ = RenderManager::Instance()->getSceneManager()->getSceneNode("n" + parent)->createChildSceneNode("n" + gameObject()->getName());
}

Transform::~Transform()
{

}

void Transform::init(const std::map<std::string, ValueType>& params)
{
	if (params.at("parent").s == "") node_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + gameObject()->getName());
	else node_ = RenderManager::Instance()->getSceneManager()->getSceneNode("n" + params.at("parent").s)->createChildSceneNode("n" + gameObject()->getName());
	setPosition(params.at("position_x").f, params.at("position_y").f, params.at("position_z").f);
	setDirection(params.at("direction_x").f, params.at("direction_y").f, params.at("direction_z").f);
	setScale(params.at("scale_x").f, params.at("scale_y").f, params.at("scale_z").f);
	setEnabled(params.at("enabled").b);
}
