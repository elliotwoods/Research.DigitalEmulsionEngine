// xiSample.cpp : program that captures 10 images and finish
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)",place,res);throw;}

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

bool continueKey() {
	cout << "Take another frame? (y): ";
	char key = getchar();
	return (key == 10 || key == 'y');
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Sample for XIMEA API V2.10
	HANDLE xiH = NULL;
	XI_RETURN stat = XI_OK;

	try {
		// Make the variables
		int roiY, roiHeight, exposure, maxExposure;

		// Get number of camera devices
		DWORD dwNumberOfDevices = 0;
		cout << "//-----Getting device count-----//" << endl;
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
		cout << "//-----Opening device-----//" << endl;
		stat = xiOpenDevice(0, &xiH);
		HandleResult(stat,"xiOpenDevice");

		// image buffer
		XI_IMG image;
		image.size = sizeof(XI_IMG);
		image.bp = NULL;
		image.bp_size = 0;

		// Get bandwidth available and limit
		int availableBandwidth;
		int limitBandwidth;
		availableBandwidth = xiGetParamInt(xiH, XI_PRM_AVAILABLE_BANDWIDTH, 0);
		limitBandwidth = xiGetParamInt(xiH, XI_PRM_LIMIT_BANDWIDTH, 0);
		
		// Begin acquisition
		cout << "//-----Starting acquisition.-----//" << endl;
		stat = xiStartAcquisition(xiH);
		HandleResult(stat,"xiStartAcquisition");

		bool capture = true;

		while (capture) {
			cout << "Capturing frame." << endl;
			// getting image from camera
			stat = xiGetImage(xiH, 1000, &image);
			HandleResult(stat,"xiGetImage");

			capture = continueKey();
		}
	} catch (...) {

	}

	// Close device
	if (xiH)
		xiCloseDevice(xiH);
}

