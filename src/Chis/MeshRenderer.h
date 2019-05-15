#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include <Engine.h>

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void onDisable();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();
	inline Ogre::Entity* getEntity(){ return entity_; }
	Ogre::MeshPtr getMesh() { return entity_->getMesh(); }
	void changeMesh(std::string meshName, std::string materialName);

private:
	static std::string name_;

	Ogre::Entity* entity_;
	std::string meshName_;
	std::string materialName_;
};

class MeshRendererFactory : public BaseFactory
{
public:
	MeshRendererFactory() {}
	virtual ~MeshRendererFactory() {}

	virtual Component* create() { return new MeshRenderer(); }
};

#endif // !__MESH_RENDERER_H__