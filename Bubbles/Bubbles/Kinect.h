#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

class Kinect{
	public:
		HANDLE rgbStream;
		HANDLE depthStream;
		INuiSensor* sensor;

		Kinect(void);
		bool initialiseKinect(void);
};