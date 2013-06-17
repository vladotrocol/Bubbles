#ifndef _POINTGREY_IMAGE_PROCESSER
#define _POINTGREY_IMAGE_PROCESSER

#include "./Prerequisites/MustardTrackingPrerequisites.h"
#include "./Prerequisites/MustardPrivateDependencies.h"
#include "./ImageProcesser.h"
#include "./FileReaders/GenericConfigReader/GenericConfigReader.h"

//=============================================================================
// Project Includes
//=============================================================================
#include <flycapture2.h>

using namespace MustardTracking;

class PointGreyImageProcesser: public ImageProcesser{
	int camID;
	//Image data
	IplImage *Image ;
	FlyCapture2::Error   error;
	// This acts as a handle to the camera.
	FlyCapture2::Camera cam;
	FlyCapture2::CameraInfo camInfo;
	int width, height, channels,bitsPerColor;
	std::string pixelFormat;
	bool showImage;
	//Status data
	int status;
	static const int NONE=0;
	static const int CONFIGURED=1;
	static const int RUNNING=2;
	//Thread data
	pthread_mutex_t mutex;
	pthread_t thread;
	float waitTime;
	

public:
	PointGreyImageProcesser();

	PointGreyImageProcesser(const PointGreyImageProcesser& ocv);

	PointGreyImageProcesser& operator=(const PointGreyImageProcesser& ocv);

	virtual ~PointGreyImageProcesser();

	virtual void configure(std::string file);

	virtual void start();

	virtual void stop();

	virtual void getImage(int* width,int* height, int* channels,int* bitsPerColor,std::string* pixelformat,char* data);

	void run();
private:
	void notifyFPS(int ips,int cps){
		printf("IPS %d / cycles %d \n",ips,cps);
	}

	void updateFPS(bool newImage);
};


#endif