#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// OGRE Includes
#include <OgreBitesPrerequisites.h>
#include <OgreFileSystemLayer.h>
#include <OgreBuildSettings.h>
#include <OgreFrameListener.h>
#include <OgreLogManager.h>
#include <OgrePlugin.h>
#include <OgreRoot.h>

// OGRE Bites Includes
#include <OgreSGTechniqueResolverListener.h>
#include <OgreShaderGenerator.h>
#include <OgreOverlaySystem.h> 
#include <OgreInput.h>

struct SDL_Window;
struct NativeWindowPair {
	Ogre::RenderWindow* render = nullptr;
	SDL_Window* native = nullptr;
};

class Application : public Ogre::FrameListener
{
public:
	explicit Application(const Ogre::String appName);
	virtual ~Application();

	void init();
	void close();

	Ogre::Root* getRoot() const { return mRoot; }
	Ogre::RenderWindow* getRenderWindow() const { return mWindow.render; }
	Ogre::OverlaySystem* getOverlaySystem() const { return mOverlaySystem; }

protected:
	virtual void setup();
	virtual void shutdown();

	void addInputListener(OgreBites::InputListener* lis) { mInputListeners.insert(lis); }
	void removeInputListener(OgreBites::InputListener* lis) { mInputListeners.erase(lis); };
	
private:
	virtual void locateResources();
	virtual void createRoot();

	bool initialiseRTShaderSystem();
	void destroyRTShaderSystem();

	// Window handling methods and callbacks
	virtual void createWindow(const Ogre::String& windowName);
	virtual void setWindowGrab(bool grab);
	virtual void windowMoved(Ogre::RenderWindow* rw) {}
	virtual void windowResized(Ogre::RenderWindow* rw) {}
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {}
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	virtual void windowClosed(Ogre::RenderWindow* rw) {}

	// Event handling methods
	virtual bool frameStarted(const Ogre::FrameEvent& evt) { pollEvents(); return true; }
	virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void _fireInputEvent(const OgreBites::Event& event) const;
	void pollEvents();

protected:
	// Application window
	NativeWindowPair mWindow;
	Ogre::String mAppName;
	bool mFirstRun;

	// OGRE Systems
	Ogre::Root* mRoot;
	Ogre::OverlaySystem* mOverlaySystem;
	Ogre::FileSystemLayer* mFileSystemLayer;
	Ogre::RTShader::ShaderGenerator* mShaderGenerator;
	OgreBites::SGTechniqueResolverListener* mMaterialMgrListener;
	std::set<OgreBites::InputListener*> mInputListeners;

	// Paths
	Ogre::String mSolutionPath;
	Ogre::String mRTShaderLibPath;
};

#endif