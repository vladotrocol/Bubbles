#ifndef _WATER_DISPLAY
#define _WATER_DISPLAY

#include "./WaterDisplay/WaterLayerInfo.h"
#include <vector>
#include <string>

class FieldData;

class WaterDisplayInfo{
public:
	std::vector<WaterLayerInfo> layers;

	static WaterDisplayInfo readCallibration(std::string file="callibration.cfg");

	static WaterDisplayInfo readCallibration(FieldData* config);

	/**
		Saves the water display configuration into a file. 
		@param contents: Configuration to save
		@param file: Name of the file where the contents will be saved
		@param fileMode: Describes how the file will be openned ("w" to overwrite previous file, "a" to add configuration to the current file)
	*/		
	static void writeCallibration(WaterDisplayInfo& contents, std::string file="callibration.cfg", std::string fileMode="w");
};


#endif
