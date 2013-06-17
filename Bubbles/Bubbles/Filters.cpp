#include "Filters.h"

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

int threshold_value = 1;
int threshold_type = THRESH_BINARY_INV;

//Constructor
Filters::Filters():thresholdOn(false),
	erosionOn(false),
	dilationOn(false)
{};


//Applies the requested filter to the corresponding source stream
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
		threshold_value = 1;
		threshold_type = THRESH_BINARY_INV;
		threshold(src, result, threshold_value, 255, threshold_type);
	}
	else{
		threshold_value = 50;
		threshold_type = THRESH_BINARY;
		cvtColor(src, src_grey, CV_BGR2GRAY);
		threshold(src_grey, result, threshold_value, 255, threshold_type);
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
