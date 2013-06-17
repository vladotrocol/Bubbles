#include "WDDLayer.h"
#include "./WaterDisplayUSBController/COMToolkit.h"
#include <iostream>
#include <math.h>

using namespace std;


WDDLayer::~WDDLayer(){
	;
}

WDDLayer::WDDLayer(int layerID):status(INIT),layerID (layerID){
	;
}

void WDDLayer::setOnPeriod(int period[]){
	if(period[0]<0 || period[1]<0 || period[2]<0)
		return;
	this->on=period[0]*100+period[1]*10+period[2];
	WDDLayer::sendData(COMToolkit::ON_PERIOD, 3, period);
	cout << "ON period set for layer " << layerID << endl ;
}


void WDDLayer::setOffset(int time[]){
	if(time[0]<0 || time[1]<0 || time[2]<0)
		return;
	this->offset=time[0]*100+time[1]*10+time[2];
	WDDLayer::sendData(COMToolkit::OFFSET, 3, time);
	cout << "OFFSET set for layer " << layerID << endl ;
}


void WDDLayer::setCycleTime(int time[]){
	if(time[0]<0 || time[1]<0 || time[2]<0 || time[3]<0)
		return;
	this->period=time[0]*1000+time[1]*100+time[2]*10+time[3];
	WDDLayer::sendData(COMToolkit::CYCLE_TIME, 4, time);
	cout << "CYCLE TIME set for layer " << layerID << endl ;
}


/*void WDDLayer::setCorrespondency(float points[][2]){
	for(int i=0; i<4; i++){
		for(int j=0; j<2; j++){
			this->HomographyPoints[i][j] = points[i][j];
		}
	}
}*/



void WDDLayer::configureLayer(int layer[]){
	this->on=layer[0]*100+layer[1]*10+layer[2];
	this->offset=layer[3]*100+layer[4]*10+layer[5];
	this->period=layer[6]*100+layer[7]*10+layer[8];

	WDDLayer::sendData(COMToolkit::CONFIG_LAYER, 9, layer);
	cout << "Layer " << layerID << " configured." << endl ;
}


void WDDLayer::loadLayer(){
	COMToolkit::readByte();
	COMToolkit::sendByte(COMToolkit::LOAD_LAYER);
	COMToolkit::readByte();
	COMToolkit::sendByte(layerID);
	cout << "Layer " << layerID << " loaded." << endl ;
}

void WDDLayer::startLayer(){
	COMToolkit::readByte();
	COMToolkit::sendByte(COMToolkit::START);
	COMToolkit::readByte();
	COMToolkit::sendByte(layerID);
	cout << "Layer " << layerID << " started." << endl ;
}


void WDDLayer::stopLayer(){
	COMToolkit::readByte();
	COMToolkit::sendByte(COMToolkit::STOP);
	COMToolkit::readByte();
	COMToolkit::sendByte(layerID);
	if(layerID == 0){
		cout << "All layers stopped." << endl ;
	}
	else{
		cout << "Layer " << layerID << " stopped." << endl ;
	}
}


void WDDLayer::sendData(int option, int numOfBytes, int data[]){

	char bytes[4] = {'0'};
	int i = 0;
	
	switch(option){
		case COMToolkit::ON_PERIOD:
			while(!COMToolkit::readByte()){}
			COMToolkit::sendByte(COMToolkit::ON_PERIOD);
			break;
		case COMToolkit::OFFSET:
			while(!COMToolkit::readByte()){}
			COMToolkit::sendByte(COMToolkit::OFFSET);
			break;
		case COMToolkit::CYCLE_TIME:
			while(!COMToolkit::readByte()){}
			COMToolkit::sendByte(COMToolkit::CYCLE_TIME);
			break;
		case COMToolkit::CONFIG_LAYER:
			while(!COMToolkit::readByte()){}
			COMToolkit::sendByte(COMToolkit::CONFIG_LAYER);
			break;
	}

	while(!COMToolkit::readByte()){}
	COMToolkit::sendByte(layerID);

	for(int i=0; i < numOfBytes && COMToolkit::readByte(); i++){
		COMToolkit::sendByte(data[i]);
	}
}


