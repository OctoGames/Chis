#include "Component.h"
#include "EntityComponentManager.h"

Component::Component() :
	first_enabled_(true),
	enabled_(true),
	container_(nullptr)
{
}

Component::~Component()
{
}

void Component::setEnabled(bool enabled)
{
	enabled_ = enabled;

	if (first_enabled_ && enabled_)
	{
		EntityComponentManager::Instance()->addComponent(this);
		first_enabled_ = false;
		start();
	}
	else
	{
		enabled_ ? onEnabled() : onDisabled();
	}
}
