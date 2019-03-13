#pragma once

#include "component.h"
#include "OgreSystem.h"

class Camera : public Component
{
public:
	Camera(GameObject* container, bool enabled);
	virtual ~Camera();

	virtual std::string getName() const { return name_; }

private:
	//Poninter to the main camera and viewPort
	Ogre::Camera* mainCamera;
	Ogre::Viewport* viewport;

	//Scene nodes to the camera and lights
	Ogre::SceneNode* mCamNode_ = nullptr;

	void initCamera();

	static std::string name_;
};