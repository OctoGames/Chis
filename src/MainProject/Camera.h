#pragma once

#include "Component.h"
#include "RenderManager.h"

class Camera : public Component
{
public:
	Camera(GameObject* container, bool enabled);
	virtual ~Camera();

	virtual std::string getName() const { return name_; }

private:
	//Poninter to the main camera and viewPort
	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;

	//Scene nodes to the camera and lights
	Ogre::SceneNode* mCamNode_ = nullptr;

	void initCamera();

	static std::string name_;
};