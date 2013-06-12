#include "Tracker.h"

Tracker::Tracker(){
};

void Tracker::display(Kinect& kinect){
	namedWindow("rgb_window",CV_WINDOW_AUTOSIZE);
	namedWindow("depth_window",CV_WINDOW_AUTOSIZE);
	while(1){
		Mat rgb_temp,depth_temp;
		while (rgb_temp.empty())
			rgb_temp = readFrameRGB(kinect);
		rgb_temp.copyTo(rgb_src);
		imshow("rgb_window",rgb_src);
		int a = cvWaitKey(10);
		while(depth_temp.empty())
			depth_temp = readFrameDepth(kinect);
		depth_temp.copyTo(depth_src);
		imshow("depth_window",depth_src);
		int b = cvWaitKey(10);
		rgb_temp.release();
		depth_temp.release();
	}

};

Mat Tracker::readFrameDepth(Kinect& kinect){
	Mat depthFrame(height, width, CV_8U);

	NUI_LOCKED_RECT LockedRect;
	BYTE data[width*height*3];
	NUI_IMAGE_FRAME imageFrame;

	if (kinect.sensor->NuiImageStreamGetNextFrame(kinect.depthStream, 5, &imageFrame) < 0) 
	{
		cout << "No Dframe available\n";
		return Mat();
	}

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0,&LockedRect,NULL,0);

	if (LockedRect.Pitch != 0){
		int j = 0;
		const USHORT* curr = (const USHORT*) LockedRect.pBits;
		const USHORT* dataEnd = curr + (width*height);
		while (curr < dataEnd) {
			// Get depth in millimeters
			USHORT depth = NuiDepthPixelToDepth(*curr++);
			// Draw a grayscale image of the depth:
			// B,G,R are all set to depth%256, alpha set to 1.
			for (int i = 0; i < 3; ++i){
				data[j] = (BYTE) depth%256;
			}
			j++;
		}
		Mat depthF(height, width, CV_8U, data);
		depthF.copyTo(depthFrame);
	}
	else{
		cout<<"Pitch is 0";
		return Mat();
	}
	texture->UnlockRect(0);
	kinect.sensor->NuiImageStreamReleaseFrame(kinect.depthStream, &imageFrame);

	flip(depthFrame, depthFrame, 1);

	return depthFrame;
};

Mat Tracker::readFrameRGB(Kinect& kinect){
	Mat rgbFrame(height, width, CV_8UC4);

	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (kinect.sensor->NuiImageStreamGetNextFrame(kinect.rgbStream, 10, &imageFrame) < 0) {
		cout << "No Rframe available\n";
		return Mat();
	}
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0,&LockedRect,NULL,0);

	if (LockedRect.Pitch != 0){
		Mat rgbF(height, width, CV_8UC4, LockedRect.pBits);
		rgbF.copyTo(rgbFrame);
	}
	else{
		return Mat();
	}
	texture->UnlockRect(0);
	kinect.sensor->NuiImageStreamReleaseFrame(kinect.rgbStream, &imageFrame);
	rgbFrame.convertTo(rgbFrame, CV_8UC3);
	return rgbFrame.clone();
}
