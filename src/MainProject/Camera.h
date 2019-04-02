#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Component.h"
#include "RenderManager.h"

class Camera : public Component
{
public:
	Camera(GameObject* container, bool enabled);
	virtual ~Camera();

	virtual std::string getName() const { return name_; }

private:
	Ogre::Camera* camera_;
	Ogre::Viewport* viewport_;

	Ogre::SceneNode* mCamNode_ = nullptr;

	void initCamera();

	static std::string name_;
};

#endif // !__CAMERA_H__