#include "Filters.h"

//Constructor
Filters::Filters():thresholdOn(false),
	erosionOn(false),
	dilationOn(false)
{};

Mat Filters::applyFilter(char s, Mat src){
	if(s == 't'){
		return thresholdFilter(src);
	}else if(s == 'e'){
		return erosionFilter(thresholdFilter(src));
	}else if(s == 'i'){
		return dilationFilter(thresholdFilter(src));
	}else{
		return src;
	}
};

//Apply Threshold Filer
Mat Filters::thresholdFilter(Mat src){
	Mat src_grey, result;
	if(src.type() == CV_8U){
		threshold(src, result, 1, 255, THRESH_BINARY_INV);
	}
	else{
		cvtColor(src, src_grey, CV_BGR2GRAY);
		threshold(src_grey, result, 50, 255, THRESH_BINARY);
	}
	return result;
};

//Apply Erosion Filter
Mat Filters::erosionFilter(Mat src){
	Mat result;
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10), Point(2,2));
	erode( src, result, element );
	return result;
};

//Apply Dilation Filter
Mat Filters::dilationFilter(Mat src){
	Mat result;
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10), Point(2,2));
	dilate( src, result, element );
	return result;
};