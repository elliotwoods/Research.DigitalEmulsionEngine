#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1280, 1024);
	
	gui.init();
	gui.add(debugPixels, "Debug pixes");
	gui.add(decoder.getCameraInProjector(), "Camera in projector");
	gui.add(decoder.getProjectorInCamera(), "Projector in camera");
	gui.add(decoder.getMedian(), "Median");
	
	payload.init(1280, 1024);
	decoder.init(payload);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	static int threshold = 10;
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 's')
		decoder.saveDataSet();
	
	if (key == OF_KEY_DOWN) {
		threshold -= 2;
		cout << "Threshold : " << threshold;
		decoder.setThreshold(threshold);
	}
	if (key == OF_KEY_UP) {
		threshold+= 2;
		cout << "Threshold : " << threshold;
		decoder.setThreshold(threshold);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	auto files = dragInfo.files;	
	int width, height;
	
	for (auto filename : files) {
		ofImage rootBranch;
		rootBranch.loadImage(filename);
		
		cout << "Adding set " << filename << endl;
		
		int frameWidth = rootBranch.getWidth() / 4;
		int frameHeight = rootBranch.getHeight() / 6;
		
		cout << "Frame width : \t" << frameWidth << endl;
		cout << "Frame height : \t" << frameHeight << endl;
		
		ofPixels frameRedGreen;
		frameRedGreen.allocate(frameWidth, frameHeight, 3);
		
		ofPixels capture;
		capture.allocate(frameWidth, frameHeight, 1);
		
		for (int i=0; i<21; i++) {
			int x = i % 4;
			int y = i / 4;
			rootBranch.getPixelsRef().cropTo(frameRedGreen, x * frameWidth, y * frameHeight, frameWidth, frameHeight);
			
			for (int pix=0; pix<frameWidth*frameHeight; pix++) {
				capture[pix] = frameRedGreen[pix * 4 + 1];
			}
			
			decoder << capture;
			debugPixels = frameRedGreen;
		}
		
		decoder.saveDataSet(ofFilePath::getBaseName(filename));
	}
}