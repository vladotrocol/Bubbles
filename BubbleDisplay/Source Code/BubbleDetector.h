#include <iostream>
#include "BubbleState.h"
#include "KOCVStream.h"

class IBubbleDetector{
	public:
		virtual bool init(void)=0;
		virtual bool start(void)=0;
		virtual void run(void)=0;
		virtual bool stop(void)=0;
};

class BubbleDetector: public IBubbleDetector{

	public:
		BubbleDetector(void);
		bool init(void);
		bool start(void);
		void run(void);
		bool stop(void);
};