#include "./CallibrationEngine/IEventObserver.h"
#include "./WaterDisplayUSBController/XMOSControlPlugin.h"

class OnEventCallUp:public IEventObserver{
	XMOSControlPlugin& proxy;
public:
	OnEventCallUp(XMOSControlPlugin& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.up();
	}
};

class OnEventCallDown:public IEventObserver{
	XMOSControlPlugin& proxy;
public:
	OnEventCallDown(XMOSControlPlugin& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.down();
	}
};

class OnEventCallLeft:public IEventObserver{
	XMOSControlPlugin& proxy;
public:
	OnEventCallLeft(XMOSControlPlugin& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.left();
	}
};

class OnEventCallRight:public IEventObserver{
	XMOSControlPlugin& proxy;
public:
	OnEventCallRight(XMOSControlPlugin& proxy):proxy(proxy){;}
	virtual void notifyEvent(){
		proxy.right();
	}
};