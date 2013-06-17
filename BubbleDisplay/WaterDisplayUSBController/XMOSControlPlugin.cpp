#include "./WaterDisplayUSBController/XMOSControlPlugin.h"
#include "./WaterDisplayUSBController/XMOSPluginEventObservers.h"
#include "./CallibrationEngine/IEventProducer.h"
#include "./WaterDisplayUSBController/XmosConfig.h"

XMOSControlPlugin::XMOSControlPlugin():status(SELECT_LAYER), curLayer(1){
	layerStarted=new bool[XmosConfig::instance().numLayers()];
	for(int i=0;i<XmosConfig::instance().numLayers();i++)
		layerStarted[i]=false;
}

void XMOSControlPlugin::config(IEventProducer& mainApp){
	
	mainApp.registerEventObserver('i', (IEventObserver*)(new OnEventCallUp(*this)));
	mainApp.registerEventObserver('k', (IEventObserver*)(new OnEventCallDown(*this)));
	mainApp.registerEventObserver('j', (IEventObserver*)(new OnEventCallLeft(*this)));
	mainApp.registerEventObserver('l', (IEventObserver*)(new OnEventCallRight(*this)));
}

std::string XMOSControlPlugin::getHelpString(){
	static char s_msg[256];
	WDDLayer& l=XmosConfig::instance().getLayer(curLayer);
	switch(status){
		case SELECT_LAYER:
			sprintf(s_msg, "L%d(%.1f,%.1f,%.1f):   selectLayer('i'/'k'); run('j'); config('l')",curLayer, (l.getOnPeriod() / 10.0), (l.getOffset() / 10.0), (l.getCycleTime() / 10.0));
			break;
		case START_LAYER:
			sprintf(s_msg, "L%d(%.1f,%.1f,%.1f):   start/stop('i'/'k'); selectLayer('j'); onTime('l')",curLayer, (l.getOnPeriod() / 10.0), (l.getOffset() / 10.0), (l.getCycleTime() / 10.0));
			break;
		case ON_PERIOD:
			sprintf(s_msg, "L%d(%.1f,%.1f,%.1f):   changeOn('i'/'k'); start('j'); offset('l')",curLayer, (l.getOnPeriod() / 10.0), (l.getOffset() / 10.0), (l.getCycleTime() / 10.0));
			break;
		case OFFSET:
			sprintf(s_msg, "L%d(%.1f,%.1f,%.1f):   changeOffset('i'/'k'); onTime('j'); period('l')",curLayer, (l.getOnPeriod() / 10.0), (l.getOffset() / 10.0), (l.getCycleTime() / 10.0));
			break;
		case CYCLE:
			sprintf(s_msg, "L%d(%.1f,%.1f,%.1f):   changePeriod('i'/'k'); offset('l'); selectLayer('l');",curLayer, (l.getOnPeriod() / 10.0), (l.getOffset() / 10.0), (l.getCycleTime() / 10.0));
			break;
		case RUNNING:
			sprintf(s_msg, "RUNNING ALL LAYERS: Press 'l' to stop and config.");
			//ignore
			break;
	}
	return std::string(s_msg);
}

void printStatus(int status,int curLayer){
	WDDLayer& l=XmosConfig::instance().getLayer(curLayer);
	printf("%d: L%d(%d,%d,%d)\n", status, curLayer, l.getOnPeriod(), l.getOffset(), l.getCycleTime());


}

void XMOSControlPlugin::up(){
	printStatus(status, curLayer);
	switch(status){
		case SELECT_LAYER:
			curLayer--;
			if(curLayer<1)curLayer=XmosConfig::instance().numLayers();
			break;
		case START_LAYER:
			XmosConfig::instance().startLayer(curLayer);
			layerStarted[curLayer]=true;
			break;
		case ON_PERIOD:
			if(layerStarted[curLayer])
				XmosConfig::instance().stopLayer(curLayer);
			XmosConfig::instance().setOnPeriod(curLayer,XmosConfig::instance().getLayer(curLayer).getOnPeriod()+1);
			XmosConfig::instance().loadLayer(curLayer);
			if(layerStarted[curLayer])
				XmosConfig::instance().startLayer(curLayer);
			break;
		case OFFSET:
			if(layerStarted[curLayer])
				XmosConfig::instance().stopLayer(curLayer);
			XmosConfig::instance().setOffset(curLayer,XmosConfig::instance().getLayer(curLayer).getOffset()+1);
			XmosConfig::instance().loadLayer(curLayer);
			if(layerStarted[curLayer])
				XmosConfig::instance().startLayer(curLayer);
			break;
		case CYCLE:
			if(layerStarted[curLayer])
				XmosConfig::instance().startLayer(curLayer);
			XmosConfig::instance().setCycleTime(curLayer,XmosConfig::instance().getLayer(curLayer).getCycleTime()+1);
			XmosConfig::instance().loadLayer(curLayer);
			if(layerStarted[curLayer])
				XmosConfig::instance().startLayer(curLayer);
			break;
		case RUNNING:
			//ignore
			break;
	}
}

void XMOSControlPlugin::down(){
	printStatus(status, curLayer);
	switch(status){
		case SELECT_LAYER:
			curLayer=((curLayer)%XmosConfig::instance().numLayers())+1;
			break;
		case START_LAYER:
			XmosConfig::instance().stopLayer(curLayer);
			layerStarted[curLayer]=false;
			break;
		case ON_PERIOD:
			XmosConfig::instance().setOnPeriod(curLayer,XmosConfig::instance().getLayer(curLayer).getOnPeriod()-1);
			XmosConfig::instance().loadLayer(curLayer);
			break;
		case OFFSET:
			XmosConfig::instance().setOffset(curLayer,XmosConfig::instance().getLayer(curLayer).getOffset()-1);
			XmosConfig::instance().loadLayer(curLayer);
			break;
		case CYCLE:
			XmosConfig::instance().setCycleTime(curLayer,XmosConfig::instance().getLayer(curLayer).getCycleTime()-1);
			XmosConfig::instance().loadLayer(curLayer);
			break;
		case RUNNING:
			//ignore
			break;
	}
}



void XMOSControlPlugin::left(){
	printStatus(status, curLayer);
	switch(status){
		case SELECT_LAYER:
			XmosConfig::instance().loadLayer(curLayer);
			XmosConfig::instance().startAllLayers();
			status=RUNNING;
			break;
		case START_LAYER:
			status=SELECT_LAYER;
			break;
		case ON_PERIOD:
			status=START_LAYER;
			break;
		case OFFSET:
			status=ON_PERIOD;
			break;
		case CYCLE:
			status=OFFSET;
			break;
		case RUNNING:
			//ignore
			break;
	}
}



void XMOSControlPlugin::right(){
	printStatus(status, curLayer);
	switch(status){
		case SELECT_LAYER:
			status=START_LAYER;
			break;
		case START_LAYER:
			status=ON_PERIOD;
			break;
		case ON_PERIOD:
			status=OFFSET;
			break;
		case OFFSET:
			status=CYCLE;
			break;
		case CYCLE:
			status=SELECT_LAYER;
			break;
		case RUNNING:
			status=SELECT_LAYER;
			XmosConfig::instance().stopAllLayers();
			break;
	}
}

