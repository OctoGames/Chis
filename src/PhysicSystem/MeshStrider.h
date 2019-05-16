#ifndef __MESH_STRIDER_H__
#define __MESH_STRIDER_H__

#include <btBulletDynamicsCommon.h>

#include "RenderManager.h"


//Creates triangled meshes instead of boxes for the map objects 
class MeshStrider : public btStridingMeshInterface 
{
public:
	MeshStrider(Ogre::Mesh * m = nullptr) : mesh_(m) {}

	void setMesh(Ogre::Mesh * m) { if(m) mesh_ = m; }

	virtual int getNumSubParts() const;

	virtual void getLockedVertexIndexBase(unsigned char **vertexbase, int& numverts, PHY_ScalarType& type, int& stride, unsigned char **indexbase, int & indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0);
	virtual void getLockedReadOnlyVertexIndexBase(const unsigned char **vertexbase, int& numverts, PHY_ScalarType& type, int& stride, const unsigned char **indexbase, int & indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0) const;

	virtual void unLockVertexBase(int subpart);
	virtual void unLockReadOnlyVertexBase(int subpart) const;

	virtual void preallocateVertices(int numverts);
	virtual void preallocateIndices(int numindices);

private:
	Ogre::Mesh* mesh_;
};

#endif // !__MESH_STRIDER_H__