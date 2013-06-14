#include "Filters.h"

int threshold_value = 1;
int threshold_type = THRESH_BINARY_INV;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

//Constructor
Filters::Filters():thresholdOn(false),
	erosionOn(false),
	dilationOn(false)
{};

//Apply Threshold Filer
Mat Filters::thresholdFilter(Mat src){
	Mat result;
	threshold(src, result, threshold_value, max_BINARY_value, threshold_type);
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