#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <string>

using namespace std;

class Kinect{
	public:
		HANDLE rgbStream;
		HANDLE depthStream;
		INuiSensor* sensor;

		Kinect(void);
		bool initialiseKinect(void);
		bool hasNextFrame(string s, NUI_IMAGE_FRAME &imageFrame);
};