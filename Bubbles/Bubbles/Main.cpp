#include <iostream>
#include "Tracker.h"

using namespace std;

int main(){
	Tracker T;
	Kinect K;
	cout<<K.initialiseKinect();
	T.display(K);
	cin.get();
	return 0;
}