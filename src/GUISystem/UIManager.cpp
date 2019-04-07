#include "UIManager.h"

UIManager* UIManager::instance_ = nullptr;

UIManager * UIManager::Instance()
{
	if (instance_ == nullptr) instance_ = new UIManager();
	return instance_;
}

UIManager::UIManager() :
	renderer_(nullptr),
	fWnd(nullptr),
	menuClosed_(false)
{
	InputManager::Instance()->addKeyListener(this, "GUI");
	InputManager::Instance()->addMouseListener(this, "GUI");
}

void UIManager::init()
{
	// Init CEGUI System
	renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(RenderManager::Instance()->getWindow()));

	// Load CEGUI Resources
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	// Set default resources
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");


	// Create CEGUI root object
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* myRoot = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	fWnd = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
	myRoot->addChild(fWnd);
	fWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.2f, -0.2f), CEGUI::UDim(-0.2f, -0.2f)));
	fWnd->setSize(CEGUI::USize(CEGUI::UDim(1.3f, 1.3f), CEGUI::UDim(1.3f, 1.3f)));
	fWnd->setTitleBarEnabled(false);
	fWnd->setCloseButtonEnabled(false);

	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	fWnd->addChild(quit);
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quit->setText("Quit");
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&UIManager::quit, this));

	CEGUI::Window *start = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	fWnd->addChild(start);
	start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	start->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	start->setText("Start");
	start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&UIManager::closeMenu, this));
}

void UIManager::close()
{
	CEGUI::OgreRenderer::destroySystem();
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}

bool UIManager::keyPressed(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)e.key);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)e.text);
	return true;
}

bool UIManager::keyReleased(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)e.key);
	return true;
}

bool UIManager::mouseMoved(const OIS::MouseEvent & e)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseMove(e.state.X.rel, e.state.Y.rel);
	if (e.state.Z.rel) context.injectMouseWheelChange(e.state.Z.rel / 120.0f);
	return true;
}

bool UIManager::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}

bool UIManager::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool UIManager::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (RenderManager::Instance()->getWindow()->isClosed()) return false;
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	return true;
}

void UIManager::quit()
{
	RenderManager::Instance()->setRunning(false);
}

void UIManager::closeMenu()
{
	fWnd->hide();
	menuClosed_ = true;
}

void UIManager::openMenu()
{
	fWnd->show();
	menuClosed_ = false;
}