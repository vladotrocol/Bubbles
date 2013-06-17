 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h> //DIRTY dirty trick. This needs to be included first than anything else. Otherwise UDP does not work
#include "windows.h"
#endif


#include "./DisplayKernel/OgreApplications/CompositeOgreApplication.h"
#include "./DisplayKernel/OgreApplications/LayerSimulation.h"
#include "./DisplayKernel/OgreApplications/VideoPlayer.h"
#include "./WaterDisplayUSBController/XMOSControlPlugin.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
int main(int argc, char** argv)
{
	//0.Create plugins

	try {
         mainApp.go();
    } catch( Ogre::Exception& e ) {
		std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
	}
	im->stopInputSubsystems();
	engine.stopEngine();
    return 0;

}*/



