#include "Prototype.h"

Prototype::Prototype(GameObject * container, const std::list<Component*>& components) :
	container_(container), components_(components)
{
}

Prototype::~Prototype()
{
	for (Component* c : components_)
	{
		delete c;
		c = nullptr;
	}

	delete container_;
	container_ = nullptr;
}
