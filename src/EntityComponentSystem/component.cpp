#include "Component.h"
#include "EntityComponentManager.h"

Component::Component(GameObject* container, bool enabled) :
	first_enabled_(true),
	container_(container)
{
	setEnabled(enabled);
	EntityComponentManager::Instance()->addComponent(this);
}

Component::~Component()
{
	onDestroy();
}

void Component::setEnabled(bool enabled)
{
	enabled_ = enabled;

	if (first_enabled_ && enabled_)
	{
		first_enabled_ = false;
		start();
	}
	else
	{
		enabled_ ? onEnabled() : onDisabled();
	}
}