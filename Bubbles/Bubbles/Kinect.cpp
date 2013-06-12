#include "Kinect.h"

//Constructor
Kinect::Kinect(void){
};


//Initialise Kinect
bool Kinect::initialiseKinect(void){
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,2,NULL,&depthStream);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &rgbStream); 

	return sensor;
};

//Check if kinect has next frame
bool Kinect::hasNextFrame(string s, NUI_IMAGE_FRAME &imageFrame){
	HANDLE stream;
	if(s.compare("rgb") == 0){
		stream = &rgbStream;
	}
	else if(s.compare("depth")==0){
		stream = &depthStream;
	}
	else{
		return false;
	}
	if(sensor->NuiImageStreamGetNextFrame(stream, 10, &imageFrame) < 0){
		return true;
	}
	return false;
}