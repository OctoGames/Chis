#include "TimeManager.h"

TimeManager::TimeManager(Ogre::Timer* timer) :
	timer_(timer),
	lastTime_(0.0f),
	deltaTime_(0.0f)
{
	timer_->reset();
}

TimeManager::~TimeManager()
{
}

float TimeManager::tick()
{
	unsigned long lastTimeMs = lastTime_ * 1000;
	unsigned long currentTimeMs = timer_->getMilliseconds();
	unsigned long deltaTimeMs = currentTimeMs - lastTimeMs;

	lastTime_ = 0.001f * float(currentTimeMs);
	deltaTime_ = 0.001f * float(deltaTimeMs);

	return deltaTime_;
}
