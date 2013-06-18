#include "BubbleDetector.h"

//Constructor
BubbleDetector::BubbleDetector()
{};


bool BubbleDetector::init(){
	Kinect kinect;
	Filters filter;
	bool ki = kinect.initialiseKinect();
	cout<<"Kinect: "<<ki<<'\n';
	KOCVStream STREAMS(kinect,filter);
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


/*void BubbleDetector::getContours(Mat src){
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	findContours(filter.applyFilter('t',src), contours, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	/// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }


  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( filter.applyFilter('t',src).size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
};*/