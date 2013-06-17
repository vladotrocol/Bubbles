#include <iostream>
#include <map>
#include <queue>
#include <pthread.h>
#include "Bubble.h"

using namespace std;

class IBubbleState{
	public:
		virtual void addBubble(Bubble b)=0;
		virtual unsigned int getUnknownBubble(void)=0;
		virtual void updateBubble(unsigned int ID, float x, float y, float z)=0;
		virtual map<unsigned int, Bubble> getCurrentState(void)=0;
		virtual void notifyUserBreaks(unsigned int ID)=0;
		virtual void notifyTrackingFrame(void)=0;
};

class BubbleState: public IBubbleState{
	private:
		queue<Bubble> nonTrackedBubbles;
		map<unsigned int, Bubble> trackedBubbles;
		pthread_mutex_t lock;

		//Constructor
		BubbleState(void);

	public:

		//Methods
		static BubbleState& instance(void);
		void addBubble(Bubble b);
		unsigned int getUnknownBubble(void);
		void updateBubble(unsigned int ID, int x, int y, int z);
		map<unsigned int, Bubble> getCurrentState(void);
		void notifyUserBreaks(unsigned int ID);
		void notifyTrackingFrame(void);
};