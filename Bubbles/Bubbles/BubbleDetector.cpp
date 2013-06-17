#include "BubbleDetector.h"

//Constructor
BubbleDetector::BubbleDetector(){
};


bool BubbleDetector::init(){
	Kinect K;
	Filters F;
	bool ki = K.initialiseKinect();
	cout<<"Kinect: "<<ki<<'\n';
	KOCVStream STREAMS(K,F);
	return true;
};

bool BubbleDetector::start(){
	return true;
};

void BubbleDetector::run(){
};

bool BubbleDetector::stop(){
	return true;
};