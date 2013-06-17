#ifndef _IMAGE_PROCESSER
#define _IMAGE_PROCESSER
#include <vector>
#include "opencv2/imgproc/imgproc.hpp"


class   ImageProcesser{
protected:
	static const int ST_INIT=0;
	static const int ST_READY=1;
	static const int ST_PLAYING=2;
	static const int ST_PAUSE=3;
	int status;
	//Mode data
	static const int MODE_REAL_TIME=0;
	static const int MODE_ALL_FRAMES=1;
	int mode;

public:
	struct TimestampedFrame{
		cv::Mat image;
		int time;
		TimestampedFrame(cv::Mat i,int  t):image(i), time(t){;}
	};
	ImageProcesser():status(ST_INIT),mode(MODE_REAL_TIME){;}
	virtual void configure(std::string file){
		status=ST_READY;
	}
	virtual void start()=0;
	virtual void stop()=0;
	virtual void pause()=0;
	virtual void play()=0;
	inline void setRealTimeMode(){
		mode=MODE_REAL_TIME;
		dropOldFrames();
	}
	inline void setAllFramesMode(){
		mode=MODE_ALL_FRAMES;
	}
	virtual void dropOldFrames(){;}		
	virtual cv::Mat getImage()=0;
	virtual cv::Mat getImage(int& timeStamp){return getImage();};
	virtual ~ImageProcesser(){;}
};


#endif