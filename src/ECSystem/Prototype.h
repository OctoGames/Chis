#ifndef __PROTOTYPE_H__
#define __PROTOTYPE_H__

#include <list>

#include "GameObject.h"
#include "Component.h"

class Prototype
{
public:
	Prototype(GameObject* container, const std::list<Component*>& components);
	~Prototype();

	inline GameObject* getEntity() { return container_; }
	inline std::list<Component*> getComponents() { return components_; }

private:
	GameObject* container_;
	std::list<Component*> components_;
};

#endif // !__PROTOTYPE_H__

