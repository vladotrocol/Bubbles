#ifndef _POINTGREY_IMAGE_PROCESSER
#define _POINTGREY_IMAGE_PROCESSER
#include "./ImageProceser/ImageProcesser.h"
#include "./FileReaders/GenericConfigReader/GenericConfigReader.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
//=============================================================================
// Project Includes
//=============================================================================


#include <string>
#include <vector>
#include <pthread.h>


class PointGreyImageProcesser: public ImageProcesser{
	//Thread data
	pthread_mutex_t mutex;//lock while writing to avoid conflicts.
	pthread_mutex_t captureStarted;//Will be set to 1 when the first image is grabbed.
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

	virtual void pause();

	virtual void play();

	virtual void dropOldFrames();

	virtual cv::Mat getImage();

	virtual cv::Mat getImage(int& timeStamp);

	void run();

private:
	void updateFPS(bool newImage);
};


#endif