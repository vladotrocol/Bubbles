#include <iostream>
#include <Windows.h>
#include <Ole2.h>
#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Kinect.h"

//of what?
#define width 640 
#define height 480

using namespace std;
using namespace cv;

class Tracker{
	public:
		Mat src;
		bool iHomog; //What is this?

		Tracker(void);
		void calibrateCameraProjector(void);
		Mat readFrameRGB(const Kinect& kinect);
};