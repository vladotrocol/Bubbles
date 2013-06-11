#include "Tracker.h"

Tracker::Tracker(){
};

//MAKE A FUNCTION TO SHOW THAT RGBFRAMEWORKS

//Grab Kinect RGB Frame and cast it into OpenCV Mat
Mat Tracker::readFrameRGB(const Kinect& kinect){
	Mat rgbFrame(height, width, CV_8UC4);
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (kinect.sensor->NuiImageStreamGetNextFrame(kinect.rgbStream, 10, &imageFrame) < 0) 
	{
		cout << "No frame available\n";
		return Mat();
	}
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0,&LockedRect,NULL,0);

	if (LockedRect.Pitch != 0)
	{
		Mat frame(height, width, CV_8UC4, LockedRect.pBits);
		frame.copyTo(rgbFrame);
	}
	else
	{
		return Mat();
	}
	texture->UnlockRect(0);
	kinect.sensor->NuiImageStreamReleaseFrame(kinect.rgbStream, &imageFrame);
	rgbFrame.convertTo(rgbFrame, CV_8UC3);
	return rgbFrame.clone();
}
