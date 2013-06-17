#include "./ImageProceser/PointGreyImageProcesser/PointGreyImageProcesser.h"
#include <assert.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <memory.h>
#include "CallibrationEngine\IClock.h"




void* fwthreadFunction(void* a);//Helper function to create the thread



	PointGreyImageProcesser::PointGreyImageProcesser():ImageProcesser(){
		//imageNotifierCB=NULL;
		//Create locks
		pthread_mutex_init(&mutex,NULL);
		pthread_mutex_init(&captureStarted,NULL);
		pthread_mutex_lock(&captureStarted);	 //captureStarted is locked untill we unlock it.
		
	}

	PointGreyImageProcesser::~PointGreyImageProcesser(){
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&captureStarted);
	}

	void PointGreyImageProcesser::configure(std::string file){
	}

	void PointGreyImageProcesser::start(){
		if(status==ST_READY){
			status=ST_PLAYING;
			pthread_create(&thread,NULL,fwthreadFunction,(void*)this);
		}
	}

	void* fwthreadFunction(void* a){
		PointGreyImageProcesser* p=((PointGreyImageProcesser*)a);
		p->run();
		return NULL;
	}
	void PointGreyImageProcesser::run(){
		//Thread's main loop
		while(status==ST_PLAYING || status==ST_PAUSE){
				if(status==ST_PAUSE){
					Sleep(5);//Do nothing while paused. 
					continue;
				}
				//Do your processing
				this->updateFPS(true);
				//Leave the processor (do this always! You have to let other threads get the processor)
				Sleep(1);
			}
		//The status says we have to end
		Sleep(1000);//Keep it		
	}

	void PointGreyImageProcesser::stop(){
		if(status!=ST_PLAYING && status!=ST_PAUSE)
			return;
		status=ST_READY;
		//await termination
		void* result;
		pthread_join(thread,&result);
	}

	void PointGreyImageProcesser::pause(){
		if(status==ST_PLAYING)
			status=ST_PAUSE;
	}

	void PointGreyImageProcesser::play(){
		if(status!=ST_PAUSE)
			return;
		status=ST_PLAYING;
	}
	void PointGreyImageProcesser::dropOldFrames(){
		pthread_mutex_lock(&mutex);
		//Do something with mutual exclusion
		pthread_mutex_unlock(&mutex);
	}
		
	cv::Mat PointGreyImageProcesser::getImage(){
		//This lock we use it as a flag: Is the capture Started? Do not let me through otherwise
		pthread_mutex_lock(&captureStarted);			
		pthread_mutex_unlock(&captureStarted);			
		//This lock we use it to get mutual exclusion
		pthread_mutex_lock(&mutex);//Entering M.E.
		//Do something with mutual exclusion
		pthread_mutex_unlock(&mutex);//Leaving M.E.
		return cv::Mat();//copy lattest image.		
	}

	cv::Mat PointGreyImageProcesser::getImage(int& timeStamp){
		return cv::Mat();
	}

	


	void PointGreyImageProcesser::updateFPS(bool newFrame){
		static Ogre::Timer timer;
		static bool firstTime=true;
		static float lastSecond;
		static int imagesSinceLastSecond;
		static int cyclesSinceLastSecond;
		static const int measureOverNSeconds=15; //We print it over 5 seconds....
		float curSecond;
		float curTime=((float)(timer.getMicroseconds()+1)/1000000);
		//0. Initialize clock
		if(firstTime){
			timer.reset();
			imagesSinceLastSecond=cyclesSinceLastSecond=0;
			lastSecond=curSecond=1.0f*static_cast<int>(curTime);
			firstTime=false;
		}
		//1. Update number of images and cycles since last second
		cyclesSinceLastSecond++;
		if(newFrame)imagesSinceLastSecond++;	
		//2. Determine in the second is finished and notify if necessary
		curSecond=static_cast<int>(curTime);
		if(curSecond>=lastSecond+measureOverNSeconds){
			printf("%d", cyclesSinceLastSecond);
			imagesSinceLastSecond=cyclesSinceLastSecond=0;lastSecond=curSecond;
		}	
			
	}


