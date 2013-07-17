#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1400, 1050);
	payload.init(1400, 1050);
	decoder.init(payload);
	decoder.setThreshold(2);
//	gui.init();
//	gui.add(decoder.getCameraInProjector(), "Camera in projector");
//	gui.add(decoder.getProjectorInCamera(), "Projector in camera");
//	gui.add(decoder.getMedian(), "Median");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	decoder.getCameraInProjector().draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	static int threshold = 10;
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 's')
		decoder.saveDataSet();
	
	if (key == OF_KEY_DOWN) {
		threshold -= 1;
		cout << "Threshold : " << threshold;
		decoder.setThreshold(threshold);
	}
	if (key == OF_KEY_UP) {
		threshold += 1;
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
	cout << "We need " << payload.getFrameCount() << " frames for a complete set." << endl;
	
	int width, height;
	
	for (auto filename : files) {
		ofImage image;
		image.loadImage(filename);
		cout << "Adding file " << filename << endl;
		decoder << image;
	}
	median = decoder.getMedian();
	median.update();
}