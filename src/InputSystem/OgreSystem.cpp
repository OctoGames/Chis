#include "OgreSystem.h"

OgreSystem* OgreSystem::instance_ = nullptr;

OgreSystem * OgreSystem::Instance()
{
	if (instance_ == nullptr)
		instance_ = new OgreSystem();
	return instance_;
}

OgreSystem::OgreSystem() :
	mouse_(nullptr),
	keyboard_(nullptr),
	joystick_(nullptr),
	inputManager_(nullptr)
{

}