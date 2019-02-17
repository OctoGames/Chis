#include "OgreRoot.h"

int main()
{
	Ogre::Root* root;

#if _DEBUG
	root = new Ogre::Root("plugins_d.cfg");
#else
	root = new Ogre::Root("plugins.cfg");
#endif

	return 0;
}