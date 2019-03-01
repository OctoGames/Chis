#include "Transform.h"

std::string Transform::name_ = "Transform";

Transform::Transform(GameObject* container, bool enabled) : Component(container, enabled)
{
	node_ = Application::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("n" + gameObject()->getName());
}

Transform::~Transform()
{

}