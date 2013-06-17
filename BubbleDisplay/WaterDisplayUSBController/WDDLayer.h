#ifndef __WDDLayer_H__
#define __WDDLayer_H__

#include "./CallibrationEngine/Homography.h"

// Water Drop Display Layer
class WDDLayer{
	int layerID;
	int status;
	enum Layer_States{INIT=0, READY, ERRORS, END};
	//internal variables
	int offset, on, period;
	//int HomographyPoints[4][2]; 
public:
	void setOnPeriod(int period[]);
	void setOffset(int time[]);
	void setCycleTime(int time[]);
	
	//Add this to get the values back, if required
	inline int getOffset(){return offset;}
	inline int getOnPeriod(){return on;}
	inline int getCycleTime(){return period;}
	//inline float getCorrespondency(int i, int j){return HomographyPoints[i][j];}
	void computeHomography(cv::Mat& stencil);
	void configureLayer(int layer[]);
	void loadLayer();
	void startLayer();
	void stopLayer();
	~WDDLayer();
	WDDLayer(int layerID);
	//void setCorrespondency(float points[][2]);
	//inline int getCorrespondency(int pointID,int coord){ return HomographyPoints[pointID][coord];}
	//inline void setCorrespondencyPoint(int pointID,int coord, int value){HomographyPoints[pointID][coord]=value;}
private:
	
	void sendData(int option, int numOfBytes, int data[]);
};

#endif