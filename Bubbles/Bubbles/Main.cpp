#include <iostream>
#include "KOCVStream.h"

using namespace std;

int main(){
	Kinect K;
	cout<<K.initialiseKinect();
	
	KOCVStream STREAMS(K);
	STREAMS.displayAll();
	cin.get();
	return 0;
}