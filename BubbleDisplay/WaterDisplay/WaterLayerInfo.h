#ifndef _WATER_LAYER
#define _WATER_LAYER

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>

class WaterLayerInfo{
public:
	//Static information: These fields are stored in/read from the config file:
	int onTime, offset, cycle;	
	std::string outputTexture, inputTexture;
	int framesPerCycle;
	int framesPerSecond;
	int px_width, px_height;	
	//** Bydimensional array containing the stencil for each cycle and each frame: stencil[cycle:1..60][frame: 1..24]; 
	std::vector< std::vector<std::string>> stencil;
	std::vector< std::vector<cv::Mat>> stencilImg;
	//**The coordinates in pixels of the points that determine the homography
	float srcPoints[4][2];
	float dstPoints[4][2];
	//Runtime information. This field is initializated to 0 and modified during runtime. 
	int curFrame, curCycle;

	WaterLayerInfo()
		:onTime(0)
		,offset(0)
		,cycle(0)
		,framesPerCycle(1)
		,framesPerSecond(60)
		,px_width(608)
		,px_height(640)
		,curFrame(0)
		,curCycle(0)	
	{;}

};

#endif
