#include "./PointGreyImageProcesser/PointGreyImageProcesser.h"
#include "./ImageObserver.h"
#include <assert.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <memory.h>



#define INITIALIZE         0x000
#define CAMERA_POWER       0x610
#define FORMAT		       0x100
#define MODE		       0x180
#define FRAMERATE          0x214

using namespace MustardTracking;

void* fwthreadFunction(void* a);



	PointGreyImageProcesser::PointGreyImageProcesser():camID(0),width(640), height(480), channels(3), bitsPerColor(8),pixelFormat("RGB") , waitTime(0), showImage(false),status(NONE){
		Image=NULL;
		//imageNotifierCB=NULL;
		pthread_mutex_init(&mutex,NULL);		
	}

	PointGreyImageProcesser::PointGreyImageProcesser(const PointGreyImageProcesser& ocv):camID(ocv.camID),width(ocv.width), height(ocv.height), channels(ocv.channels), bitsPerColor(ocv.bitsPerColor),pixelFormat(ocv.pixelFormat) , waitTime(ocv.waitTime), showImage(ocv.showImage),status(ocv.status){;
		Image=new IplImage(*(ocv.Image));
		//imageNotifierCB=ocv.imageNotifierCB;
		observers.clear();
		observers.assign(ocv.observers.begin(),ocv.observers.end());
		pthread_mutex_init(&mutex,NULL); 
		if(pthread_mutex_trylock(const_cast<pthread_mutex_t*>(&(ocv.mutex))))//If the lock is free...
			pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&(ocv.mutex)));// do nothing (set it free again)
		else
			pthread_mutex_lock(&mutex);			//otherwise, lock this->mutex
	}

	PointGreyImageProcesser& PointGreyImageProcesser::operator=(const PointGreyImageProcesser& ocv){;
		camID=ocv.camID;
		width=ocv.width;
		height=ocv.height;
		channels=ocv.channels;
		bitsPerColor=ocv.bitsPerColor;
		pixelFormat=ocv.pixelFormat;
		waitTime=ocv.waitTime;
		showImage=ocv.showImage;
		status=ocv.status;
		IplImage* oldImage=Image;
		Image=new IplImage(*(ocv.Image));
		if(oldImage)delete oldImage;
		//imageNotifierCB=ocv.imageNotifierCB;
		observers.clear();
		observers.assign(ocv.observers.begin(),ocv.observers.end());
		pthread_mutex_destroy(&mutex);
		pthread_mutex_init(&mutex,NULL); 
		if(pthread_mutex_trylock(const_cast<pthread_mutex_t*>(&(ocv.mutex))))//If the lock is free...
			pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&(ocv.mutex)));// do nothing (set it free again)
		else
			pthread_mutex_lock(&mutex);			//otherwise, lock this->mutex
		return *this;
	}

	PointGreyImageProcesser::~PointGreyImageProcesser(){
		pthread_mutex_destroy(&mutex);
		if(Image)delete Image;
	}

	void PointGreyImageProcesser::configure(std::string file){
		GenericConfigReader c(new FileInputStream(file));
		if(!c.parse()) return;

		FieldData* config;
		if(!c.configuration.getSubfield("ImageProcesser"))
			return;
		config=c.configuration.getSubfield("ImageProcesser");

		FieldData* fd=config->getSubfield("cameraWidth");
		if(fd)
			this->width=Operation::fromStrToInt(fd->getFieldValue());

		fd=config->getSubfield("camID");
		if(fd)
			this->camID=Operation::fromStrToInt(fd->getFieldValue());

		fd=config->getSubfield("cameraHeight");
		if(fd)
			this->height=Operation::fromStrToInt(fd->getFieldValue());

		fd=config->getSubfield("cameraChannels");
		if(fd)
			this->channels=Operation::fromStrToInt(fd->getFieldValue());

		fd=config->getSubfield("bitsPerColor");
		if(fd)
			this->bitsPerColor=Operation::fromStrToInt(fd->getFieldValue());

		fd=config->getSubfield("waitTime");
		if(fd)
			this->waitTime=Operation::fromStrToFloat(fd->getFieldValue());

		fd=config->getSubfield("showImage");
		if(fd)
			this->showImage=(Operation::fromStrToInt(fd->getFieldValue())!=0);

		fd=config->getSubfield("cameraPixelFormat");
		if(fd)
			this->pixelFormat=fd->getFieldValue();

		this->status=CONFIGURED;

	}

	void PointGreyImageProcesser::start(){
		if(status!=CONFIGURED)
			return;
		status=RUNNING;
		pthread_create(&thread,NULL,fwthreadFunction,(void*)this);
		for(int i=0;i<observers.size();i++)
			observers[i]->start();
	}

	void PointGreyImageProcesser::stop(){
		for(int i=0;i<observers.size();i++)
				observers[i]->stop();
		if (status==RUNNING){
			status=CONFIGURED;
			void* result;
			pthread_join(thread,&result);
		}
	    error = cam.StopCapture();
		if (error != FlyCapture2::PGRERROR_OK)
		{
			error.PrintErrorTrace();
			return;
		}      

		// Disconnect the camera
		error = cam.Disconnect();
		if (error != FlyCapture2::PGRERROR_OK)
		{
			error.PrintErrorTrace();
			return;
		}

    return ;
}
	

	void PointGreyImageProcesser::getImage(int* width,int* height, int* channels,int* bitsPerColor,std::string* pixelformat,char* data){
		if(!this->Image){
			*width=*height=*channels=*bitsPerColor=0;
			pixelFormat="error";
			//Should throw something here...
			char c;
			printf("OpenCVImageProcesser ERROR: The image could not be captured. Call Configure first.");
			scanf ("%c",&c);

			return;
		}
		*width=this->width;
		*height=this->height;
		*channels=this->channels;
		*bitsPerColor=this->bitsPerColor;
		*pixelformat=this->pixelFormat;
		if(data){
			pthread_mutex_lock(&mutex);
			int size=this->width*this->height*this->channels*((int)this->bitsPerColor/8);
			memcpy(data,this->Image->imageData,size);
			pthread_mutex_unlock(&mutex);
		}
	}


	void PrintCameraInfo( FlyCapture2::CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}


	void PointGreyImageProcesser::run(){
		Image=cvCreateImage(cvSize( 1024,768), IPL_DEPTH_8U , channels );
		//0. Configure and start the Firewire camera   
		FlyCapture2::BusManager busMgr;
		FlyCapture2::PGRGuid guid;
		//0.1. Get the ID of the camera
		error = busMgr.GetCameraFromIndex(0, &guid);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            error.PrintErrorTrace();
            return;
        }
		//0.2 Connect to it
	    error = cam.Connect(&guid);
		if (error != FlyCapture2::PGRERROR_OK)
		{
			error.PrintErrorTrace();
			return;
		}
		//0.3 Get the camera information
	    error = cam.GetCameraInfo(&camInfo);
	    if (error != FlyCapture2::PGRERROR_OK)
	    {
	        error.PrintErrorTrace();
	        return;
	    }

	    PrintCameraInfo(&camInfo);        
		//0.4. Set camera mode and framerate
		cam.SetVideoModeAndFrameRate(FlyCapture2::VideoMode::VIDEOMODE_1024x768RGB,FlyCapture2::FrameRate::FRAMERATE_60);
		// Start capturing images
		error = cam.StartCapture();
		if (error != FlyCapture2::PGRERROR_OK)
		{
		    error.PrintErrorTrace();
		    return;
		}
		//1. Set thing so we can display the image
		char imgName[8]="Image 0";
		imgName[6]='0'+camID;
		if(showImage)cvNamedWindow(imgName, CV_WINDOW_AUTOSIZE);
		while(status==RUNNING){
			//Get a frame. If no new frame Sleep and retry
			FlyCapture2::Image rawImage;    
			// Retrieve an image
			error = cam.RetrieveBuffer( &rawImage );
			if (error != FlyCapture2::PGRERROR_OK)
			{
				error.PrintErrorTrace();
				continue;
			}
			pthread_mutex_lock(&mutex);
			Image->imageData=reinterpret_cast<char*>(rawImage.GetData());
			pthread_mutex_unlock(&mutex);
			for(int i=0;i<observers.size();i++)
				observers[i]->notifyImage(width,height,channels,bitsPerColor,(char*)pixelFormat.c_str(),Image->imageData);
			if(showImage){
				imgName[6]='0'+camID;
				cvShowImage(imgName,Image);
				cvWaitKey(1);
			}
			this->updateFPS(true);
			Sleep(waitTime);
		}
		status=CONFIGURED;
		Sleep(1000);
	}

	void PointGreyImageProcesser::updateFPS(bool newFrame){
		return;
		static Ogre::Timer timer;
		static bool firstTime=true;
		static float lastSecond;
		static int imagesSinceLastSecond;
		static int cyclesSinceLastSecond;
		float curSecond;
		float curTime=((float)(timer.getMicroseconds()+1)/1000000);
		//0. Initialize clock
		if(firstTime){
			timer.reset();
			imagesSinceLastSecond=cyclesSinceLastSecond=0;
			lastSecond=curSecond=static_cast<int>(curTime);
			firstTime=false;
		}
		//1. Update number of images and cycles since last second
		cyclesSinceLastSecond++;
		if(newFrame)imagesSinceLastSecond++;	
		//2. Determine in the second is finished and notify if necessary
		curSecond=static_cast<int>(curTime);
		if(curSecond!=lastSecond){
			lastSecond=curSecond;
			notifyFPS(imagesSinceLastSecond,cyclesSinceLastSecond);
			imagesSinceLastSecond=cyclesSinceLastSecond=0;
		}	
			
	}


void* fwthreadFunction(void* a){
	PointGreyImageProcesser* p=((PointGreyImageProcesser*)a);
	p->run();
	return NULL;
}