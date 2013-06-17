#include "BubbleDetector.h"

class IBubbleTracker{
	public:
};

class BubbleTracker: public IBubbleTracker{
	public:
		bool init(void);
		bool start(void);
		bool stop(void);
		void update(float x, float y, float z, int radius);

	private: 
		BubbleDetector& _detector;
};