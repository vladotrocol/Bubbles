#include <iostream>
#include "KOCVStream.h"

using namespace std;

int main(){
	Kinect K;
	Filters F;
	cout<<K.initialiseKinect();
	KOCVStream STREAMS(K,F);
	STREAMS.display("dtei");
	cin.get();
	return 0;
}