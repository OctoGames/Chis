#include "Transform.h"

std::string Transform::name_ = "Transform";

Transform::Transform(GameObject* container, const std::string& parent, bool enabled) : Component(container, enabled)
{
	if (parent == "") node_ = Application::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + gameObject()->getName());
	else node_ = Application::Instance()->getSceneManager()->getSceneNode("n" + parent)->createChildSceneNode("n" + gameObject()->getName());
}

Transform::~Transform()
{

}