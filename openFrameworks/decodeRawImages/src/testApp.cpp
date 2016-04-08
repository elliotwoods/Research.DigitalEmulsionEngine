#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1280, 1024);
	payload.init(1280, 1024);
	decoder.init(payload);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	decoder.getCameraInProjector().draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 's')
		decoder.saveDataSet();
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
		ofFile file;
		file.open(filename, ofFile::ReadOnly, true);
		auto buffer = file.readToBuffer();
		
		width = 2048;
		height = buffer.size() / width;
		
		ofPixels pixels;
		pixels.allocate(width, height, 1);

		pixels.setFromExternalPixels((unsigned char * ) buffer.getBinaryBuffer(), width, height, 1);
		decoder << pixels;
		cout << "Added file " << filename << endl;
		//ofSaveImage(pixels, filename + ".png");
	}
	
	median.allocate(width, height, OF_IMAGE_GRAYSCALE);
	memcpy(median.getPixels(), decoder.getMedian().getPixels(), width * height);
	median.update();
}