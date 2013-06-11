#include <iostream>
#include "Kinect.h"

using namespace std;

int main(){
	Kinect k;
	cout<<k.initialiseKinect();
	cin.get();
	return 0;
}