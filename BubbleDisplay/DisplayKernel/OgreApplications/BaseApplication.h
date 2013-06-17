
#ifndef __BaseApplication_h_
#define __BaseApplication_h_
#include <./DisplayKernel/OgreApplications/IOgreApplication.h>

class BaseApplication : public IOgreApplication, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    virtual void go(void);
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual int createViewports(int curIndex=0);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
protected: 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	// WindowEventListener
	//Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);



   
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    // OgreBites
    bool mShutDown;

};

#endif // #ifndef __BaseApplication_h_
