#include "XmosConfig.h"
#include "./WaterDisplayUSBController/COMToolkit.h"
#include <iostream>
#include <conio.h>
#include "sleep.h"

static const char  port[]="COM4";
static const char XMOS_PROGRAM[]="\"C:\\Program Files (x86)\\XMOS\\DevelopmentTools\\11.11.0\\bin\\run.bat\"";
using namespace std;


void XmosConfig::awaitSyncSignal(){
	usleep(1);
	COMToolkit::awaitSyncSignal();
}

XmosConfig::XmosConfig():status (INIT), curID(1){
	;
}

XmosConfig& XmosConfig::instance(){
	static XmosConfig _uniqueInstance;//This declaration, implicitly, calls the constructor. It can be called as this method belongs to the class. As it is a static variable, it is only created once.
	return _uniqueInstance;
}

int XmosConfig::createLayer(){
	layers.push_back(WDDLayer(curID));  //We create a layer
	curID++;							//...increase the index...
	return curID-1;						//...and return the previous value
}


XmosConfig::~XmosConfig(){
	if(status != END){
		quit();
	}
}


void  XmosConfig::establishConnection(){
	//OpenConnection through the COM port
	COMToolkit::connect();
	system("cls");
	printf("Please, turn OFF the power supply and press a key to proceed...");
	fflush(stdin);
	getch();
	//Run the executable
	system(XMOS_PROGRAM);Sleep(3000);//Let the Xmos start...
	/*char c;
	scanf("%c",&c);*/
	
	printf("Please, turn ON the power supply\n ");
	
	
}

void XmosConfig::quit(){
	this->stopAllLayers();
	COMToolkit::closeConnection();
}

void XmosConfig::startAllLayers(){
	for(int i=0;i<layers.size();i++)
		layers[i].loadLayer();
	COMToolkit::readByte();
	COMToolkit::sendByte(COMToolkit::START);
	COMToolkit::readByte();
	COMToolkit::sendByte(0);
	cout << "All layers started." << endl ;
	COMToolkit::startListeningSyncSignal();
}

void XmosConfig::stopAllLayers(){
		COMToolkit::stopListeningSyncSignal();
		for(int i=0;i<this->layers.size();i++)
			layers[i].stopLayer();
		
	}
