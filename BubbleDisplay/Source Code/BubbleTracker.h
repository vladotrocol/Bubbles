#ifndef _BUBBLE_TRACKER_C
#define _BUBBLE_TRACKER_C
#include "BubbleDetector.h"

class IBubbleTracker{
	public:
		virtual bool init(void)=0;
		virtual bool start(void)=0;
		virtual void update(float x, float y, float radius)=0;
		virtual bool stop(void)=0;
};

class BubbleTracker: public IBubbleTracker{
	public:
		//BubbleDetector _detector;
		bool init(void);
		bool start(void);
		bool stop(void);
		void update(float x, float y, float radius);

};
#endif