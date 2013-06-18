#include "BubbleState.h"

BubbleState::BubbleState(){
};

BubbleState& BubbleState::instance(){
	static BubbleState _instance;
	return _instance;
};

void BubbleState::addBubble(Bubble b){
	nonTrackedBubbles.push(b);
};

unsigned int BubbleState::getUnknownBubble(void){
	Bubble b = nonTrackedBubbles.front();
	trackedBubbles[b.ID]=b;
	return b.ID;
};

void BubbleState::updateBubble(unsigned int ID, float x, float y, float z){

};

map<unsigned int, Bubble> BubbleState::getCurrentState(void){
	map<unsigned int, Bubble> trackedBubblesCopy(trackedBubbles);
	return trackedBubblesCopy;
};

void BubbleState::notifyUserBreaks(unsigned int ID){

};

void BubbleState::notifyTrackingFrame(void){

};