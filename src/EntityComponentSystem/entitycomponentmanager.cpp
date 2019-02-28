#include "entitycomponentmanager.h"

EntityComponentManager* EntityComponentManager::instance_ = nullptr;

EntityComponentManager* EntityComponentManager::Instance()
{
	if (instance_ == nullptr) instance_ = new EntityComponentManager();
	return instance_;
}