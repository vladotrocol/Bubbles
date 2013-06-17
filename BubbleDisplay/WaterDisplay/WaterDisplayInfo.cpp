#include "./WaterDisplay/WaterDisplayInfo.h"
#include "./FileReaders/GenericConfigReader/GenericConfigReader.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
void WaterDisplayInfo::writeCallibration(WaterDisplayInfo& contents,std::string file, std::string fileMode){
	FILE* f;
	f=fopen(file.c_str(), fileMode.c_str());
	if(!f){
		printf("Error opening file %s\n", file.c_str());
		return;
	}
	fprintf(f, "WaterDisplay {\n");
	for(int i=0;i<contents.layers.size();i++){
		fprintf(f, "   Layer %d {\n",i);
		fprintf(f, "      onTime %d;\n", contents.layers[i].onTime);
		fprintf(f, "      offset %d;\n", contents.layers[i].offset);
		fprintf(f, "      cycle %d;\n", contents.layers[i].cycle);
		fprintf(f, "      framesPerCycle %d;\n", contents.layers[i].framesPerCycle);
		fprintf(f, "      cyclesPerSecond %d;\n", contents.layers[i].framesPerSecond);
		fprintf(f, "      inputTexture \"%s\";\n", contents.layers[i].inputTexture.c_str());
		fprintf(f, "      outputTexture \"%s\";\n", contents.layers[i].outputTexture.c_str());
		fprintf(f, "      px_height %d;\n", contents.layers[i].px_height);
		fprintf(f, "      px_width %d;\n", contents.layers[i].px_width);
		fprintf(f, "      SourceCorrespondency {\n");
		fprintf(f, "         x0 %f;", contents.layers[i].srcPoints[0][0]);
		fprintf(f, "         y0 %f;\n", contents.layers[i].srcPoints[0][1]);
		fprintf(f, "         x1 %f;", contents.layers[i].srcPoints[1][0]);
		fprintf(f, "         y1 %f;\n", contents.layers[i].srcPoints[1][1]);
		fprintf(f, "         x2 %f;", contents.layers[i].srcPoints[2][0]);
		fprintf(f, "         y2 %f;\n", contents.layers[i].srcPoints[2][1]);
		fprintf(f, "         x3 %f;", contents.layers[i].srcPoints[3][0]);
		fprintf(f, "         y3 %f;\n", contents.layers[i].srcPoints[3][1]);
		fprintf(f, "      }\n");
		fprintf(f, "      DestinationCorrespondency{\n");
		fprintf(f, "         x0 %f;", contents.layers[i].dstPoints[0][0]);
		fprintf(f, "         y0 %f;\n", contents.layers[i].dstPoints[0][1]);
		fprintf(f, "         x1 %f;", contents.layers[i].dstPoints[1][0]);
		fprintf(f, "         y1 %f;\n", contents.layers[i].dstPoints[1][1]);
		fprintf(f, "         x2 %f;", contents.layers[i].dstPoints[2][0]);
		fprintf(f, "         y2 %f;\n", contents.layers[i].dstPoints[2][1]);
		fprintf(f, "         x3 %f;", contents.layers[i].dstPoints[3][0]);
		fprintf(f, "         y3 %f;\n", contents.layers[i].dstPoints[3][1]);
		fprintf(f, "      }\n");
		
		fprintf(f, "      Stencils{\n");
		
		for(int cycle=0;cycle<contents.layers[i].stencil.size();cycle++){
			fprintf(f, "         cycle%d{",cycle);
			for(int frame=0;frame<contents.layers[i].stencil[cycle].size();frame++)
				fprintf(f, "texture \"%s\";", contents.layers[i].stencil[cycle][frame].c_str());
			fprintf(f, "}\n");
		}
		fprintf(f, "      }\n");
		fprintf(f, "   }\n");
		
	}
	fprintf(f, "}");
	fclose(f);
}

void readLayerCallibration(FieldData* layer, WaterDisplayInfo& wdi){
	WaterLayerInfo result;
	FieldData* fd;
	//Read atomic attributes
	result.curFrame=0;
	if(fd=layer->getSubfield("onTime"))
		result.onTime=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("offset"))
		result.offset=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("cycle"))
		result.cycle=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("framesPerCycle"))
		result.framesPerCycle=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("cyclesPerSecond"))
		result.framesPerSecond=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("inputTexture"))
		result.inputTexture=fd->getFieldValue();
	if(fd=layer->getSubfield("outputTexture"))
		result.outputTexture=fd->getFieldValue();
	if(fd=layer->getSubfield("px_width"))
		result.px_width=Operation::fromStrToInt(fd->getFieldValue());
	if(fd=layer->getSubfield("px_height"))
		result.px_height=Operation::fromStrToInt(fd->getFieldValue());
	
	//Read the four points that describe the source points of the homography:
	FieldData* correspondency, *x0,*y0,*x1,*y1,*x2,*y2,*x3,*y3;
	correspondency=layer->getSubfield("SourceCorrespondency");
	if(correspondency){
		x0=correspondency->getSubfield("x0");
		x1=correspondency->getSubfield("x1");
		x2=correspondency->getSubfield("x2");
		x3=correspondency->getSubfield("x3");
		y0=correspondency->getSubfield("y0");
		y1=correspondency->getSubfield("y1");
		y2=correspondency->getSubfield("y2");
		y3=correspondency->getSubfield("y3");	
		if(!x0 || !x1 || !x2 || !x3 || !y0 || !y1 || !y2 || !y3 )
			return;
		result.srcPoints[0][0]=Operation::fromStrToFloat(x0->getFieldValue());
		result.srcPoints[0][1]=Operation::fromStrToFloat(y0->getFieldValue());
		result.srcPoints[1][0]=Operation::fromStrToFloat(x1->getFieldValue());
		result.srcPoints[1][1]=Operation::fromStrToFloat(y1->getFieldValue());
		result.srcPoints[2][0]=Operation::fromStrToFloat(x2->getFieldValue());
		result.srcPoints[2][1]=Operation::fromStrToFloat(y2->getFieldValue());
		result.srcPoints[3][0]=Operation::fromStrToFloat(x3->getFieldValue());
		result.srcPoints[3][1]=Operation::fromStrToFloat(y3->getFieldValue());
	}
	correspondency=layer->getSubfield("DestinationCorrespondency");
	if(correspondency){
		x0=correspondency->getSubfield("x0");
		x1=correspondency->getSubfield("x1");
		x2=correspondency->getSubfield("x2");
		x3=correspondency->getSubfield("x3");
		y0=correspondency->getSubfield("y0");
		y1=correspondency->getSubfield("y1");
		y2=correspondency->getSubfield("y2");
		y3=correspondency->getSubfield("y3");	
		if(!x0 || !x1 || !x2 || !x3 || !y0 || !y1 || !y2 || !y3 )
			return;
		result.dstPoints[0][0]=Operation::fromStrToFloat(x0->getFieldValue());
		result.dstPoints[0][1]=Operation::fromStrToFloat(y0->getFieldValue());
		result.dstPoints[1][0]=Operation::fromStrToFloat(x1->getFieldValue());
		result.dstPoints[1][1]=Operation::fromStrToFloat(y1->getFieldValue());
		result.dstPoints[2][0]=Operation::fromStrToFloat(x2->getFieldValue());
		result.dstPoints[2][1]=Operation::fromStrToFloat(y2->getFieldValue());
		result.dstPoints[3][0]=Operation::fromStrToFloat(x3->getFieldValue());
		result.dstPoints[3][1]=Operation::fromStrToFloat(y3->getFieldValue());
	}
	
	//Read the stencils for each cycle and each frame:
	FieldData* stencils, *cycle, *frame;
	if(!(stencils=layer->getSubfield("Stencils")))
		return;//ERROR
	stencils->resetSubfields();
	int curCycle=0;
	while(stencils->hasNextSubfield()){
		std::ostringstream cycleID;
		std::vector<std::string> stencilsInCycle;
		cycleID<< curCycle;
		if(cycle=stencils->getNextSubfield()){
			cycle->resetSubfields();
			while(cycle->hasNextSubfield())
				stencilsInCycle.push_back(cycle->getNextSubfield()->getFieldValue());
			result.stencil.push_back(stencilsInCycle);
		}
	}
	wdi.layers.push_back(result);
	//Load all images to memory:
	int l=wdi.layers.size()-1;
	int CPS=wdi.layers[l].stencil.size();
	for(int c=0;c<CPS;c++){
		int FPC=wdi.layers[wdi.layers.size()-1].stencil[c].size();
		std::vector<cv::Mat> result;
		for(int f=0;f<FPC;f++){ //FPS = Frames per second 
			result.push_back(cv::imread("media/materials/textures/"+wdi.layers[l].stencil[c][f]));
		}
		wdi.layers[l].stencilImg.push_back(result);
	}
}

WaterDisplayInfo WaterDisplayInfo::readCallibration(std::string file){
	GenericConfigReader c(new FileInputStream(file));
	if(!c.parse()) return WaterDisplayInfo();
	//Get the WaterDisplay node
	WaterDisplayInfo result= readCallibration(&(c.configuration));
	return result;
}

WaterDisplayInfo WaterDisplayInfo::readCallibration(FieldData* file){
	WaterDisplayInfo result;	
	FieldData* config;
	if(!file->getSubfield("WaterDisplay"))
		return result;
	config=file->getSubfield("WaterDisplay");
	//Parse the fields of this node.
	config->resetSubfields();
	while(config->hasNextSubfield()){
		readLayerCallibration(config->getNextSubfield(),result);
	}
	return result;
}