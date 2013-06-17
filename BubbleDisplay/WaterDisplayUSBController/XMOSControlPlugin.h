#ifndef _XMOS_CONTROL_PLUGIN
#define _XMOS_CONTROL_PLUGIN
#include "CallibrationEngine\IPlugin.h"

class XMOSControlPlugin:public IPlugin{
	int status;
	enum XMOSControlPluginStatus{SELECT_LAYER=0, START_LAYER, OFFSET, ON_PERIOD, CYCLE, RUNNING};
	int curLayer;
	bool* layerStarted;
public:
	XMOSControlPlugin();
	virtual void config(IEventProducer& mainApp);
	virtual std::string getHelpString();
	void up();
	void down();
	void left();
	void right();
};


#endif