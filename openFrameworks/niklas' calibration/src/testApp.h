#pragma once

#include "ofMain.h"
#include "ofxMachineVision.h"
#include "ofxXimea.h"
#include "ofxCv.h"


#define COMPUTER_DISP_WIDTH 1680
#define COMPUTER_DISP_HEIGHT 1050

// RESOLUTION OF CAMERA AND PROJECTOR: 
// - ideally, this should also be in a file (in particular if we want to calibrate several cameras/projectors). 
// Resolution of the projector:
#define PROJ_WIDTH   1024 
#define PROJ_HEIGHT  768

// Resolution of the camera (or at least resolution at which we want to calibrate it):
#define CAM_WIDTH 2048
#define CAM_HEIGHT 2048


enum CalibState {CAMERA_ONLY, CAMERA_AND_PROJECTOR_PHASE1, CAMERA_AND_PROJECTOR_PHASE2, AR_DEMO};
class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void advance();
	void reset();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    ofxMachineVision::SimpleGrabber<ofxXimea::Device> camera;

	void initialization(CalibState initialmode); 
    
	ofVideoGrabber cam;
	ofImage undistorted;
	ofImage dispimg;
    
 	ofPixels previous; 
	ofPixels diff;
	float diffMean;
	
	float lastTime;
	bool manualAcquisition, manualGetImage;
    bool displayAR;
    bool newBoardAquired;
    bool dynamicProjection;
    bool dynamicProjectionInside;
	bool semiFixedMode;
	bool active;
	
      ofTrueTypeFont	verdana14;
    
  // For tests:
    ofVideoPlayer 		eyeMovie;
    
    // VARIABLES and METHODS THAT SHOULD BELONG TO A STEREO-CALIBRATION OBJECT (probably using multiple cameras and projectors)
	ofRectangle viewportComputer, viewportProjector;
    ofxCv::Calibration calibrationCamera, calibrationProjector;
    CalibState stateCalibration;
    
    //Extrinsics (should belong to the Stereo calibration object)
    cv::Mat rotCamToProj, transCamToProj; // in fact, there should be one pair of these for all the possible pairs camera-projector, camera-camera, projector-projector. 
    string extrinsics;
    void saveExtrinsics(string filename, bool absolute = false) const;
    void loadExtrinsics(string filename, bool absolute = false);
};
