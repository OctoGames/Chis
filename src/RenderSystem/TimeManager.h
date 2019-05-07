#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include <OgreTimer.h>

class TimeManager
{
public:
	TimeManager(Ogre::Timer* timer = nullptr);
	~TimeManager();

	float tick();

	inline float deltaTime() const { return deltaTime_; }

private:
	Ogre::Timer* timer_;
	float lastTime_;
	float deltaTime_;
};

#endif // !__TIME_MANAGER_H__

