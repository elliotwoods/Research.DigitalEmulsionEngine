// xiSample.cpp : program that captures 10 images and finish
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)",place,res);goto finish;}

bool checkStatus(XI_RETURN status, string action) {
	if (status == XI_OK) {
		return true;
	} else {
		cout << "Error at " << action << " (" << status << "), skipping test." << endl;
		return false;
	}
}
 
Nanoseconds GetTime() {
	return AbsoluteToNanoseconds(UpTime());
}

float IntervalToFps(uint64_t interval) {
	return float(1e9) / float(interval);
}

uint64_t toInt(const Nanoseconds & value) {
	return * (uint64_t*) & value;
}

void runExperiment(HANDLE xiH, int exposure, int roiY, int roiHeight, int iterations) {

	XI_RETURN stat = XI_OK;

	// Make a buffer for writing string to
	stringstream log;
	
	// image buffer
	XI_IMG image;
	image.size = sizeof(XI_IMG);
	image.bp = NULL;
	image.bp_size = 0;

	// Set experiment parameters
	stat = xiSetParamInt(xiH, XI_PRM_HEIGHT, (int) roiHeight);
	HandleResult(stat, "set ROI height");

	stat = xiSetParamInt(xiH, XI_PRM_OFFSET_Y, (int) roiY);
	HandleResult(stat, "set ROI y");
	
	// Setting "exposure" parameter (10ms=10000us)
	stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, exposure);
	HandleResult(stat,"xiSetParam (exposure set)");


	// Get bandwidth available and limit
	int availableBandwidth;
	int limitBandwidth;
	availableBandwidth = xiGetParamInt(xiH, XI_PRM_AVAILABLE_BANDWIDTH, 0);
	limitBandwidth = xiGetParamInt(xiH, XI_PRM_LIMIT_BANDWIDTH, 0);
	
	// Begin acquisition
	stat = xiStartAcquisition(xiH);
	HandleResult(stat,"xiStartAcquisition");

	for (int images=0;images < iterations;images++)
	{
		// getting image from camera
		stat = xiGetImage(xiH, 1000, &image);
		HandleResult(stat,"xiGetImage");

		Nanoseconds thisFrame = GetTime();
		static Nanoseconds lastFrame = thisFrame;
		uint64_t interval = toInt(thisFrame) - toInt(lastFrame);
		lastFrame = thisFrame;

        uint64_t thisFrameTimeStamp = image.tsUSec * 1000;
        static uint64_t lastFrameTimeStamp = thisFrameTimeStamp;
        uint64_t intervalTimeStamp = thisFrameTimeStamp - lastFrameTimeStamp;
        lastFrameTimeStamp = thisFrameTimeStamp;

		stringstream logline;
		logline << availableBandwidth << "\t" << limitBandwidth << "\t" << exposure << "\t" << roiY << "\t" << roiHeight << "\t" << image.nframe << "\t" << interval << "\t" << intervalTimeStamp << endl;

		log << logline.str();
	}

finish:

	// End acquisition
	stat = xiStopAcquisition(xiH);
	HandleResult(stat,"xiStopAcquisition");

	cout << log.str();

	//skip log if blank
	if (log.str().size() == 0)
		return;

	// Write file
	char filename[50];
#ifdef XCODE
	sprintf(filename, "../../data/%d-%d-%d.txt", roiY, roiHeight, exposure);
#else
	sprintf(filename, "../../data/%d-%d-%d.txt", roiY, roiHeight, exposure);
#endif
	ofstream file;
	file.open(filename);
	file << log.str();
	file.close();	
}

double getEstimatedMaxExposure(int roiHeight) {
	const double rowsPerSec = 90 * 2048;
	const double framesPerSec = rowsPerSec / (double) roiHeight;
	
	const double interval = 1.0f / framesPerSec;
	return interval * double(1e6);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Sample for XIMEA API V2.10
	HANDLE xiH = NULL;
	XI_RETURN stat = XI_OK;

	// Make the variables
	int roiY, roiHeight, exposure, maxExposure;

	// Get number of camera devices
	DWORD dwNumberOfDevices = 0;
	stat = xiGetNumberDevices(&dwNumberOfDevices);
	HandleResult(stat,"xiGetNumberDevices (no camera found)");
	
	if (!dwNumberOfDevices)
	{
		printf("No camera found\n");
		return 0;
	} else {
		cout << "ximea API found " << dwNumberOfDevices << " devices" << endl;
	}
	
	// Retrieving a handle to the camera device 
	stat = xiOpenDevice(0, &xiH);
	HandleResult(stat,"xiOpenDevice");

	roiY = 0;

	//loop through experiment set
	for (roiHeight = 1536; roiHeight <= 2048; roiHeight+= 64) {
		maxExposure = getEstimatedMaxExposure(roiHeight) * 2.0;
		for (exposure = 10; exposure <= maxExposure; exposure += (maxExposure - 100) / 30) {
			runExperiment(xiH, exposure, roiY, roiHeight, 100);
		}
	}

	finish:
	// Close device
	if (xiH)
		xiCloseDevice(xiH);
}

