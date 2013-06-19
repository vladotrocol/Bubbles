#include <iostream>
#include "KOCVStream.h"
#include <assert.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <memory.h>
#include "sleep.h"
#include <string>
#include <vector>
#include <pthread.h>

class IBubbleDetector{
	public:
		virtual bool init(void)=0;
		virtual bool start(void)=0;
		virtual void run(void)=0;
		virtual bool stop(void)=0;
};

#define minBubbleSize 10
#define maxBubbleSize 100

class BubbleDetector: public IBubbleDetector{

	public:
		vector<Bubble> Bubbles;
		Filters filter;
		Kinect kinect;

		BubbleDetector(void);
		bool init(void);
		bool start(void);
		void run(void);
		bool stop(void);
		vector<Bubble> detectBubbles(Filters filter, Mat src);
		void updateFPS(bool newFrame);
};