#include "KOCVStream.h"

//Constructor
KOCVStream::KOCVStream(Kinect& k, Filters& f):kinect(k),filter(f)
{};

//----------------------------API------------------------

//Display streams in windows
//FLAG grammar is: ((r||d)(t||e||i)*)*
void KOCVStream::display(char* s){
	string b = "_window_";
	generateControls();
	generateWindows(s);
	while(1){
		int j=0;
		while(j<strlen(s)){
			if(s[j]=='r'||s[j]=='d'){
				readFrame(s[j]);
			}
			else{
				cerr<<"Some flag error in display function";
				break;
			}
			int i=j;
			do{
				imshow(s[j]+b+s[i], filter.applyFilter(s[i],*whichSource(s[j])));
				waitKey( 20 );
				i++;
			}while(s[i]!='r'&&s[i]!='d'&&i<strlen(s));
			j=i;
		}	
		char c = waitKey( 20 );
		//If escape is pressed exit
		if( (char)c == 27 ){
			break; 
		}	
	}
};

//Reads the current frames into the source materials
void KOCVStream::readFrame(char s){
	Mat temp;
	while (temp.empty())
		temp = tryReadFrame(s);
	if(s=='r'){
		temp.copyTo(rgb_src);
	}
	else if(s=='d'){
		temp.copyTo(depth_src);
	}
	temp.release();
};

//--------------------------Internal----------------------------

//Read the kinect frame data into a image frame
Mat KOCVStream::tryReadFrame(char s){
	NUI_IMAGE_FRAME imageFrame;

	if (!kinect.hasNextFrame(s, &imageFrame)) {
		return Mat();
	}

	Mat depthFrame = kFrameToMat(s, imageFrame);
	kinect.releaseFrame(s, &imageFrame);

	if(s == 'd'){
		flip(depthFrame, depthFrame, 1);
	}

	return depthFrame;
};

//Put the frame data onto a Mat
Mat KOCVStream::kFrameToMat(char s, NUI_IMAGE_FRAME imageFrame){
	int dataType = whichDataType(s);
	Mat frame(height, width, dataType);
	NUI_LOCKED_RECT LockedRect;

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	BYTE* imageData;
	imageData = whichImageData(s,&LockedRect);
	if (LockedRect.Pitch != 0 && imageData){
		Mat tempMat(height, width, dataType, imageData);
		tempMat.copyTo(frame);
	}
	else{
		return Mat();
	}

	texture->UnlockRect(0);

	if(dataType == CV_8UC4){
		frame.convertTo(frame, CV_8UC3);
	}

	return frame;
};

//Get the image data correspondig to flag
BYTE* KOCVStream::whichImageData(char s, NUI_LOCKED_RECT *LockedRect){
	BYTE* data;
	if(s == 'r'){
		data = (*LockedRect).pBits;
	}
	else if(s == 'd'){
		data = kinect.getDepthData(LockedRect);
	}
	else{
		cerr<<"Flag 1 is incorrect";
		data =  NULL;
	}
	return data;
};

//This function returns the correct data format for each stream type
int KOCVStream::whichDataType(char s){
	if(s=='r')
		return CV_8UC4;
	else if(s == 'd'){
		return CV_8U;
	}
	else{
		cerr<<"invalid datatype";
		return -1;
	}
};

//Returns the correct source
Mat* KOCVStream::whichSource(char s){
	if(s == 'r'){
		return &rgb_src;
	}
	else if(s == 'd'){
		return &depth_src;
	}
	else{
		cerr<<"Incorrect source";
		return NULL;
	}
};


//Generates windows according to grammar
void KOCVStream::generateWindows(char* s){
string b = "_window_";
	int j=0;
	while(j<strlen(s)){
			if(s[j]=='r'||s[j]=='d'){
				readFrame(s[j]);
			}
			else{
				cerr<<"Some flag error in display function";
				break;
			}
			int i=j;
			do{
				namedWindow(s[j]+b+s[i],0);
				i++;
			}while(s[i]!='r'&&s[i]!='d'&&i<strlen(s));
			j=i;
		}	
};

void KOCVStream::generateControls(){
	namedWindow("Controls", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar( "ThreValue", "Controls",
                  &(filter.thresholdValue), 255,
                  NULL);
	cvCreateTrackbar("ThreType","Controls",
				  &filter.thresholdType,
                  4, NULL);
	cvCreateTrackbar("EroSize","Controls",
				  &filter.erosionSize,
                  200, NULL);
	cvCreateTrackbar("DilSize","Controls",
				  &filter.dilationSize,
                  200, NULL);
};