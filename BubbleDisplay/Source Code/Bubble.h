#ifndef _BUBBLE_C
#define _BUBBLE_C

#include <opencv2\opencv.hpp>

using namespace cv;

class Bubble {
public:
	unsigned int ID;
	Point2f center;
	float radius;
	float state;
	
	/*Bubble():ID(0),center(0,0), radius(0), state(0){;}
	Bubble(Bubble& b):ID(b.ID),center(b.center.x,b.center.y), radius(b.radius), state(b.state){
		;
	}
	Bubble& operator=(const Bubble& b){
		ID=b.ID; center=b.center; radius=b.radius; state=b.state;
	}
	*/
	
} ;

#endif