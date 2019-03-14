#include "debugDrawer.h"

debugDrawer* debugDrawer::instance_ = nullptr;

debugDrawer::debugDrawer()
{

}

debugDrawer* debugDrawer::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new debugDrawer();
	}
	return instance_;
}