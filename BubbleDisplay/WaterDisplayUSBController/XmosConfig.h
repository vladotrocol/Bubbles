#ifndef __XMOSCONFIG_H__
#define __XMOSCONFIG_H__

#include "./WaterDisplayUSBController/WDDLayer.h"
#include "vector"

class XmosConfig{
private:
	//Internal status
	enum Xmos_States{INIT=0, READY, ERRORS, END};
	int status;
	//Layers:
	std::vector<WDDLayer> layers;
	int curID;
	//enum layers{LAYER_ONE = 1, LAYER_TWO, LAYER_THREE, LAYER_FOUR};
	/**
		The Singleton pattern is one of the very rare situations where you will find a private constructor. 
		The only public method that allows you to (indirectly) call this method is the instance() method.
	*/
	XmosConfig();
public:
	static XmosConfig& instance();
	~XmosConfig();
	void establishConnection();
	/**
		This is an example of factory method. XmosConfig knows about some constraints that need to be kept when creating a WDDLayer.
		Thus, instead of letting the client create them, XmosConfig creates them making sure the rules apply.
	*/
	int createLayer();
	inline WDDLayer& getLayer(int layerID){
		return layers[layerID-1];
	}
	inline int numLayers(){
		return layers.size();
	}
	inline void setOnPeriod(int layerID, int period[]){
		layers[layerID-1].setOnPeriod(period);
	}
	inline void setOffset(int layerID, int time[]){
		layers[layerID-1].setOffset(time);
	}
	inline void setCycleTime(int layerID, int time[]){
		layers[layerID-1].setCycleTime(time);
	}
	//This is just to make my life easier
	inline void setOnPeriod(int layerID, int time){
		
		int p[3];
		p[2]=time%10;//lower digit
		p[1]=((time-p[2])/10)%10;//second digit
		p[0]=((time-p[1])/100)%10;//third digit
		layers[layerID-1].setOnPeriod(p);
	}
	inline void setOffset(int layerID, int time){
		int p[3];
		p[2]=time%10;//lower digit
		p[1]=((time-p[2])/10)%10;//second digit
		p[0]=((time-p[1])/100)%10;//third digit
		layers[layerID-1].setOffset(p);
	}
	inline void setCycleTime(int layerID, int time){
		int p[4];
		p[3]=time%10;//lower digit
		time=(time-p[3])/10;
		p[2]=time%10;//lower digit
		time=(time-p[2])/10;
		p[1]=time%10;//second digit
		time=(time-p[1])/10;
		p[0]=(time)%10;//third digit
		layers[layerID-1].setCycleTime(p);
	}
	inline void configureLayer(int layerID, int layer[]){
		layers[layerID-1].configureLayer(layer);
	}
	inline void loadLayer(int layerID ){
		layers[layerID-1].loadLayer();
	}
	inline void startLayer(int layerID){
		layers[layerID-1].startLayer();
	}
	inline void stopLayer(int layerID){
		layers[layerID-1].stopLayer();
	}
	void quit();
	void startAllLayers();
	void stopAllLayers();

	static void awaitSyncSignal();
};

#endif